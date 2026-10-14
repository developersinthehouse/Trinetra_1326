#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "index.h"

// TRINETRA ESP32 communication hub. Camera feed is proxied from the laptop/camera AP.
WebServer server(80);
WiFiServer cameraServer(81);
const char* AP_SSID = "TRINETRA_1326";
const char* AP_PASSWORD = "12345678";
const char* CAMERA_SSID = "TUFAAN";
const char* CAMERA_PASSWORD = "12345678";

struct CamSrc {
  IPAddress ip;
  uint16_t port;
  const char* path;
  const char* hostHeader;
};
CamSrc CAM_FRONT = {IPAddress(192,168,1,100), 8080, "/stream",  "192.168.1.100:8080"};
CamSrc CAM_BACK  = {IPAddress(192,168,1,100), 8080, "/stream2", "192.168.1.100:8080"};
uint8_t activeCam = 0;
CamSrc& currentCam(){ return activeCam ? CAM_BACK : CAM_FRONT; }

// Aapke bataye gaye naye pin mappings ke anusار:
// RedRat (Radar): TX to RX0 (Pin 3), aur TX0 (Pin 1)
#define RADAR_RX 3   // RX0
#define RADAR_TX 1   // TX0

// Uno: RX to TX2 (Pin 17), aur TX to RX2 (Pin 16)
#define UNO_RX 16    // RX2
#define UNO_TX 17    // TX2

HardwareSerial RadarSerial(0); // UART0 for RX0/TX0
HardwareSerial UnoSerial(2);   // UART2 for RX2/TX2

String radarLine, unoLine;
int radarAngle=90, radarDistance=249, targetAngle=90, targetDistance=0;
bool targetDetected=false, targetLocked=false, relayState=false, radarRunning=true;
char currentMovement='S';
long frontDistance=999;
bool frontObstacle=false, unoOnline=false;
unsigned long lastRadarPacket=0,lastUnoPacket=0,lastWifiRetry=0,lastDemoStep=0;
bool demoRunning=false;
int demoStage=0;
String demoStatus="IDLE";
WiFiClient viewer, upstream;
bool cameraStreaming=false;
unsigned long lastCameraByte=0;
bool lastCamStaConnected=false;

// ---------- BOUNDED PRINT HELPERS ----------
void sendUNO(const String &s){
  UnoSerial.print(s);
  UnoSerial.print('\n');
  Serial.println(F("========================================"));
  Serial.print(F("[ESP32 ➔ UNO SENT] Command: "));
  Serial.println(s);
  Serial.println(F("========================================"));
}

void sendRadar(const char *cmd){
  RadarSerial.print(cmd);
  RadarSerial.print('\n');
  Serial.println(F("========================================"));
  Serial.print(F("[ESP32 ➔ RADAR SENT] Command: "));
  Serial.println(cmd);
  Serial.println(F("========================================"));
}

void logHttp(const char *path){
  Serial.println(F("----------------------------------------"));
  Serial.print(F("[HTTP REQUEST RECEIVED] Path: "));
  Serial.print(path);
  if(server.args()>0){
    Serial.print(F(" ?"));
    for(int i=0;i<server.args();i++){
      if(i) Serial.print('&');
      Serial.print(server.argName(i));
      Serial.print('=');
      Serial.print(server.arg(i));
    }
  }
  Serial.println();
  Serial.println(F("----------------------------------------"));
}

void logHttpBlocked(const char *path, const char *why){
  Serial.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
  Serial.print(F("[HTTP BLOCKED] Path: "));
  Serial.print(path);
  Serial.print(F(" | Reason: "));
  Serial.println(why);
  Serial.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
}

void sendMove(char c){
  Serial.println(F("========================================"));
  Serial.print(F(" Movement requested: "));
  Serial.println(c);
  Serial.println(F("========================================"));
  sendUNO(String(c));
  currentMovement=c;
}

void relayOn(){
  Serial.println(F("========================================"));
  Serial.println(F(" RELAY_ON requested"));
  Serial.println(F("========================================"));
  sendUNO("RELAY:1");
  relayState=true;
}

void relayOff(){
  Serial.println(F("========================================"));
  Serial.println(F(" RELAY_OFF requested"));
  Serial.println(F("========================================"));
  sendUNO("RELAY:0");
  relayState=false;
}

void startRadar(){
  Serial.println(F("========================================"));
  Serial.println(F(" Radar START requested"));
  Serial.println(F("========================================"));
  radarRunning=true;
  sendRadar("START");
}

void stopRadar(){
  Serial.println(F("========================================"));
  Serial.println(F(" Radar STOP requested"));
  Serial.println(F("========================================"));
  radarRunning=false;
  sendRadar("STOP");
}

void lockTarget(){
  if(!targetDetected){
    logHttpBlocked("/lock","NO_TARGET_DETECTED");
    server.send(409,"text/plain","NO_TARGET_DETECTED");
    return;
  }
  targetLocked=true;
  targetAngle=radarAngle;
  targetDistance=radarDistance;
  Serial.println(F("========================================"));
  Serial.print(F("[LOCK SUCCESS] Angle: "));
  Serial.print(targetAngle);
  Serial.print(F(" | Distance: "));
  Serial.println(targetDistance);
  Serial.println(F("========================================"));
  sendUNO("LOCK:"+String(targetAngle));
  server.send(200,"text/plain","LOCK_SENT:"+String(targetAngle));
}

void unlockTarget(){
  targetLocked=false;
  Serial.println(F("========================================"));
  Serial.println(F("[UNLOCK] Target released/unlocked"));
  Serial.println(F("========================================"));
  sendUNO("UNLOCK");
}

void stopDemo(){
  demoRunning=false;
  demoStage=0;
  demoStatus="DEMO STOPPED";
  Serial.println(F("========================================"));
  Serial.println(F("[DEMO] Stopped"));
  Serial.println(F("========================================"));
  sendMove('S');
  relayOff();
}

void startDemo(){
  demoRunning=true;
  demoStage=0;
  lastDemoStep=millis();
  demoStatus="STARTING";
  Serial.println(F("========================================"));
  Serial.println(F("[DEMO] Started"));
  Serial.println(F("========================================"));
}

void updateDemo(){
  if(!demoRunning || millis()-lastDemoStep<1200) return;
  lastDemoStep=millis();
  Serial.print(F("[DEMO STAGE] Stage "));
  Serial.print(demoStage);
  Serial.print(F(" ➔ "));
  switch(demoStage){
    case 0: sendMove('F'); demoStatus="FORWARD"; Serial.println(F("FORWARD")); break;
    case 1: sendMove('S'); demoStatus="STOP"; Serial.println(F("STOP")); break;
    case 2: sendMove('B'); demoStatus="BACKWARD"; Serial.println(F("BACKWARD")); break;
    case 3: sendMove('S'); demoStatus="STOP"; Serial.println(F("STOP")); break;
    case 4: sendMove('L'); demoStatus="LEFT"; Serial.println(F("LEFT")); break;
    case 5: sendMove('S'); demoStatus="STOP"; Serial.println(F("STOP")); break;
    case 6: sendMove('R'); demoStatus="RIGHT"; Serial.println(F("RIGHT")); break;
    case 7: sendMove('S'); relayOn(); demoStatus="RELAY ON"; Serial.println(F("RELAY ON")); break;
    case 8: relayOff(); demoStatus="RELAY OFF"; Serial.println(F("RELAY OFF")); break;
    default:
      Serial.println(F("END"));
      stopDemo();
      return;
  }
  demoStage++;
}

void readRadar(){
  while(RadarSerial.available()){
    char c=(char)RadarSerial.read();
    if(c=='\n'){
      radarLine.trim();
      if(radarLine.startsWith("RADAR,")){
        int c1=radarLine.indexOf(',');
        int c2=radarLine.indexOf(',',c1+1);
        if(c1>0&&c2>c1){
          int a=radarLine.substring(c1+1,c2).toInt();
          int d=radarLine.substring(c2+1).toInt();
          if(a>=0&&a<=180&&d>=0&&d<=249){
            radarAngle=a;
            radarDistance=d;
            lastRadarPacket=millis();
            if(!targetLocked){
              targetDetected=(d>=2&&d<=150);
              if(targetDetected){targetAngle=a;targetDistance=d;}
            }
          }
        }
      }else if(radarLine.length()){
        Serial.println(F("----------------------------------------"));
        Serial.print(F("[RADAR RECEIVED ←] "));
        Serial.println(radarLine);
        Serial.println(F("----------------------------------------"));
      }
      radarLine="";
    }else if(c!='\r'&&radarLine.length()<64) radarLine+=c;
    else if(radarLine.length()>=64) radarLine="";
  }
}

void readUno(){
  while(UnoSerial.available()){
    char c=(char)UnoSerial.read();
    if(c=='\n'){
      unoLine.trim();
      if(unoLine.startsWith("DIST:")){
        frontDistance=unoLine.substring(5).toInt();
        frontObstacle=frontDistance<=10;
        lastUnoPacket=millis();
        unoOnline=true;
      }else if(unoLine.startsWith("MOVE:")){
        String m=unoLine.substring(5);
        if(m.length()) currentMovement=m.charAt(0);
        lastUnoPacket=millis();
        unoOnline=true;
        Serial.println(F("++++++++++++++++++++++++++++++++++++++++"));
        Serial.print(F("[UNO RECEIVED ← MOVE CONFIRMATION]: "));
        Serial.println(unoLine);
        Serial.println(F("++++++++++++++++++++++++++++++++++++++++"));
      }else if(unoLine=="RELAY_ON"){
        relayState=true;
        lastUnoPacket=millis();
        unoOnline=true;
        Serial.println(F("++++++++++++++++++++++++++++++++++++++++"));
        Serial.println(F("[UNO RECEIVED ←] RELAY_ON Confirmed"));
        Serial.println(F("++++++++++++++++++++++++++++++++++++++++"));
      }else if(unoLine=="RELAY_OFF"){
        relayState=false;
        lastUnoPacket=millis();
        unoOnline=true;
        Serial.println(F("++++++++++++++++++++++++++++++++++++++++"));
        Serial.println(F("[UNO RECEIVED ←] RELAY_OFF Confirmed"));
        Serial.println(F("++++++++++++++++++++++++++++++++++++++++"));
      }else if(unoLine.startsWith("OBSTACLE_")){
        frontObstacle=true;
        if(unoLine.indexOf("DONE")>=0) currentMovement='S';
        lastUnoPacket=millis();
        unoOnline=true;
        Serial.println(F("++++++++++++++++++++++++++++++++++++++++"));
        Serial.print(F("[UNO RECEIVED ← OBSTACLE ALERT]: "));
        Serial.println(unoLine);
        Serial.println(F("++++++++++++++++++++++++++++++++++++++++"));
      }else if(unoLine=="TARGET_UNLOCKED"){
        targetLocked=false;
        lastUnoPacket=millis();
        unoOnline=true;
        Serial.println(F("++++++++++++++++++++++++++++++++++++++++"));
        Serial.println(F("[UNO RECEIVED ←] TARGET_UNLOCKED Confirmed"));
        Serial.println(F("++++++++++++++++++++++++++++++++++++++++"));
      }else if(unoLine.startsWith("TARGET_LOCKED:")){
        targetLocked=true;
        targetAngle=unoLine.substring(13).toInt();
        lastUnoPacket=millis();
        unoOnline=true;
        Serial.println(F("++++++++++++++++++++++++++++++++++++++++"));
        Serial.print(F("[UNO RECEIVED ← TARGET LOCK STATUS]: "));
        Serial.println(unoLine);
        Serial.println(F("++++++++++++++++++++++++++++++++++++++++"));
      }else if(unoLine.length()){
        lastUnoPacket=millis();
        unoOnline=true;
        Serial.println(F("++++++++++++++++++++++++++++++++++++++++"));
        Serial.print(F("[UNO RECEIVED ← OTHER MESSAGE]: "));
        Serial.println(unoLine);
        Serial.println(F("++++++++++++++++++++++++++++++++++++++++"));
      }
      unoLine="";
    }else if(c!='\r'&&unoLine.length()<64) unoLine+=c;
    else if(unoLine.length()>=64) unoLine="";
  }
  if(millis()-lastUnoPacket>2500){
    if(unoOnline){
      unoOnline=false;
      Serial.println(F("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
      Serial.println(F("[WARNING] Uno went offline (no packet >2.5s)"));
      Serial.println(F("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
    }
  }
}

void sendData(){
  String movement="STOP";
  if(currentMovement=='F') movement="FORWARD";
  else if(currentMovement=='B') movement="BACKWARD";
  else if(currentMovement=='L') movement="LEFT";
  else if(currentMovement=='R') movement="RIGHT";
  String j="{";
  j+="\"angle\":"+String(radarAngle);
  j+=",\"distance\":"+String(radarDistance);
  j+=",\"targetAngle\":"+String(targetAngle);
  j+=",\"targetDistance\":"+String(targetDistance);
  j+=",\"movement\":\""+movement+"\"";
  j+=",\"target\":"+(targetDetected?String("true"):String("false"));
  j+=",\"locked\":"+(targetLocked?String("true"):String("false"));
  j+=",\"front\":"+String(frontDistance);
  j+=",\"frontObstacle\":"+(frontObstacle?String("true"):String("false"));
  j+=",\"unoOnline\":"+(unoOnline?String("true"):String("false"));
  j+=",\"radarOnline\":"+((lastRadarPacket>0&&millis()-lastRadarPacket<2500)?String("true"):String("false"));
  j+=",\"relay\":"+(relayState?String("true"):String("false"));
  j+=",\"demo\":"+(demoRunning?String("true"):String("false"));
  j+=",\"demoStatus\":\""+demoStatus+"\"";
  j+=",\"radarRunning\":"+(radarRunning?String("true"):String("false"));
  j+=",\"cam\":\""+String(activeCam?"back":"front")+"\"";
  j+="}";
  server.send(200,"application/json",j);
}

void handleMove(){
  logHttp("/move");
  if(demoRunning){
    logHttpBlocked("/move","DEMO_RUNNING");
    server.send(409,"text/plain","DEMO_RUNNING");
    return;
  }
  if(!server.hasArg("cmd")||server.arg("cmd").length()!=1){
    logHttpBlocked("/move","BAD_COMMAND");
    server.send(400,"text/plain","BAD_COMMAND");
    return;
  }
  char c=server.arg("cmd")[0];
  if(c!='F'&&c!='B'&&c!='L'&&c!='R'&&c!='S'){
    logHttpBlocked("/move","BAD_COMMAND");
    server.send(400,"text/plain","BAD_COMMAND");
    return;
  }
  sendMove(c);
  server.send(200,"text/plain","OK");
}

void handleCameraClient(WiFiClient incoming){
  String requestLine=incoming.readStringUntil('\n');
  requestLine.trim();
  while(incoming.connected()){
    String h=incoming.readStringUntil('\n');
    if(h=="\r"||h.length()==0) break;
  }
  Serial.print(F("[CAM] viewer request: "));
  Serial.println(requestLine);
  if(!requestLine.startsWith("GET ")){
    Serial.println(F("[CAM] 405 Method Not Allowed"));
    incoming.print("HTTP/1.1 405 Method Not Allowed\r\nConnection: close\r\n\r\n");
    incoming.stop();
    return;
  }
  if(upstream) upstream.stop();
  upstream.setTimeout(2000);
  CamSrc &src = currentCam();
  Serial.print(F("[CAM] active="));
  Serial.print(activeCam ? F("BACK") : F("FRONT"));
  Serial.print(F(" ➔ "));
  Serial.print(src.ip);
  Serial.print(':');
  Serial.print(src.port);
  Serial.println(src.path);
  if(!upstream.connect(src.ip, src.port)){
    Serial.println(F("[CAM] 502 Bad Gateway — camera source unavailable"));
    incoming.print("HTTP/1.1 502 Bad Gateway\r\nContent-Type: text/plain\r\nConnection: close\r\n\r\nCamera source unavailable. Check ESP32 STA connection and camera URL.");
    incoming.stop();
    return;
  }
  upstream.print("GET ");
  upstream.print(src.path);
  upstream.print(" HTTP/1.1\r\nHost: ");
  upstream.print(src.hostHeader);
  upstream.print("\r\nConnection: keep-alive\r\n\r\n");
  viewer=incoming;
  viewer.setNoDelay(true);
  upstream.setNoDelay(true);
  cameraStreaming=true;
  lastCameraByte=millis();
  Serial.println(F("[CAM] stream started"));
}

void serviceCamera(){
  if(!cameraStreaming){
    WiFiClient c=cameraServer.available();
    if(c) handleCameraClient(c);
    return;
  }
  if(!viewer.connected()){
    Serial.println(F("[CAM] viewer disconnected — stream stop"));
    viewer.stop();
    upstream.stop();
    cameraStreaming=false;
    return;
  }
  WiFiClient next=cameraServer.available();
  if(next){
    Serial.println(F("[CAM] new viewer — switching stream"));
    viewer.stop();
    upstream.stop();
    cameraStreaming=false;
    handleCameraClient(next);
    return;
  }
  uint8_t buf[1024];
  int loops=0;
  while(upstream.available()&&loops++<4){
    int n=upstream.read(buf,sizeof(buf));
    if(n<=0) break;
    if(viewer.write(buf,n)==0){
      Serial.println(F("[CAM] write failed — stream stop"));
      viewer.stop();
      upstream.stop();
      cameraStreaming=false;
      return;
    }
    lastCameraByte=millis();
  }
  if((!upstream.connected()&&!upstream.available())||millis()-lastCameraByte>12000){
    Serial.println(F("[CAM] upstream timeout/closed — stream stop"));
    viewer.stop();
    upstream.stop();
    cameraStreaming=false;
  }
}

void setup(){
  Serial.begin(115200);
  delay(200);
  Serial.println();
  Serial.println(F("========================================"));
  Serial.println(F(" TRINETRA ESP32 hub — BOUNDED DEBUG ON"));
  Serial.println(F("========================================"));

  // Custom pins initialized:
  RadarSerial.begin(9600, SERIAL_8N1, RADAR_RX, RADAR_TX);
  UnoSerial.begin(9600, SERIAL_8N1, UNO_RX, UNO_TX);
  
  Serial.print(F("[BOOT] RadarSerial (UART0) initialized on RX:"));
  Serial.print(RADAR_RX);
  Serial.print(F(" TX:"));
  Serial.println(RADAR_TX);
  
  Serial.print(F("[BOOT] UnoSerial (UART2) initialized on RX:"));
  Serial.print(UNO_RX);
  Serial.print(F(" TX:"));
  Serial.println(UNO_TX);

  WiFi.mode(WIFI_AP_STA);
  WiFi.setSleep(false);
  WiFi.softAPConfig(IPAddress(192,168,4,1),IPAddress(192,168,4,1),IPAddress(255,255,255,0));
  WiFi.softAP(AP_SSID,AP_PASSWORD);
  Serial.print(F("[WIFI] Dashboard AP: "));
  Serial.print(AP_SSID);
  Serial.print(F(" / "));
  Serial.println(AP_PASSWORD);
  Serial.println(F("[WIFI] Dashboard URL: http://192.168.4.1/"));

  Serial.print(F("[WIFI] Connecting STA to camera AP: "));
  Serial.println(CAMERA_SSID);
  WiFi.begin(CAMERA_SSID,CAMERA_PASSWORD);
  unsigned long start=millis();
  while(WiFi.status()!=WL_CONNECTED&&millis()-start<10000){delay(250);}
  if(WiFi.status()==WL_CONNECTED){
    lastCamStaConnected=true;
    Serial.print(F("[WIFI] Camera STA IP: "));
    Serial.println(WiFi.localIP());
  }else{
    lastCamStaConnected=false;
    Serial.println(F("[WIFI] WARNING: camera AP not connected; will retry in loop"));
  }

  server.on("/",[]{
    logHttp("/");
    server.send_P(200,"text/html",REMOTE_HTML);
  });
  server.on("/data",sendData);
  server.on("/move",handleMove);
  server.on("/relay/on",[]{
    logHttp("/relay/on");
    if(demoRunning){
      logHttpBlocked("/relay/on","DEMO_RUNNING");
      server.send(409,"text/plain","DEMO_RUNNING");
      return;
    }
    relayOn();
    server.send(200,"text/plain","ON");
  });
  server.on("/relay/off",[]{
    logHttp("/relay/off");
    relayOff();
    server.send(200,"text/plain","OFF");
  });
  server.on("/fire/on",[]{
    logHttp("/fire/on (alias→relay)");
    if(demoRunning){
      logHttpBlocked("/fire/on","DEMO_RUNNING");
      server.send(409,"text/plain","DEMO_RUNNING");
      return;
    }
    relayOn();
    server.send(200,"text/plain","ON");
  });
  server.on("/fire/off",[]{
    logHttp("/fire/off (alias→relay)");
    relayOff();
    server.send(200,"text/plain","OFF");
  });
  server.on("/demo/start",[]{
    logHttp("/demo/start");
    startDemo();
    server.send(200,"text/plain","OK");
  });
  server.on("/demo/stop",[]{
    logHttp("/demo/stop");
    stopDemo();
    server.send(200,"text/plain","OK");
  });
  server.on("/lock",[]{
    logHttp("/lock");
    if(demoRunning){
      logHttpBlocked("/lock","DEMO_RUNNING");
      server.send(409,"text/plain","DEMO_RUNNING");
      return;
    }
    lockTarget();
  });
  server.on("/unlock",[]{
    logHttp("/unlock");
    unlockTarget();
    server.send(200,"text/plain","OK");
  });
  server.on("/radar/start",[]{
    logHttp("/radar/start");
    startRadar();
    server.send(200,"text/plain","OK");
  });
  server.on("/radar/stop",[]{
    logHttp("/radar/stop");
    stopRadar();
    server.send(200,"text/plain","OK");
  });
  server.on("/camera",[]{
    logHttp("/camera");
    if(!server.hasArg("src")){
      server.send(200,"text/plain", activeCam ? "back" : "front");
      return;
    }
    String s=server.arg("src");
    s.toLowerCase();
    if(s=="front"||s=="0"||s=="f"){
      activeCam=0;
      Serial.println(F("[CAM] switch → FRONT"));
      if(cameraStreaming){viewer.stop();upstream.stop();cameraStreaming=false;}
      server.send(200,"text/plain","front");
    }else if(s=="back"||s=="1"||s=="b"){
      activeCam=1;
      Serial.println(F("[CAM] switch → BACK"));
      if(cameraStreaming){viewer.stop();upstream.stop();cameraStreaming=false;}
      server.send(200,"text/plain","back");
    }else{
      server.send(400,"text/plain","BAD_SRC use front|back");
    }
  });
  server.onNotFound([]{
    Serial.print(F("[HTTP 404 NOT FOUND] Path: "));
    Serial.println(server.uri());
    server.send(404,"text/plain","Not found");
  });

  server.begin();
  cameraServer.begin();
  Serial.println(F("[BOOT] HTTP server :80, camera proxy :81 ready"));

  Serial.println(F("[BOOT] Initializing: RELAY off + radar START"));
  sendUNO("RELAY:0");
  startRadar();
  Serial.println(F("[BOOT] System Fully Ready."));
}

void loop(){
  readRadar();
  readUno();
  server.handleClient();
  serviceCamera();
  updateDemo();

  bool sta=WiFi.status()==WL_CONNECTED;
  if(sta!=lastCamStaConnected){
    lastCamStaConnected=sta;
    if(sta){
      Serial.print(F("[WIFI] Camera STA connected, IP="));
      Serial.println(WiFi.localIP());
    }else{
      Serial.println(F("[WIFI] Camera STA lost"));
    }
  }
  if(!sta&&millis()-lastWifiRetry>7000){
    lastWifiRetry=millis();
    Serial.print(F("[WIFI] Retry STA → "));
    Serial.println(CAMERA_SSID);
    WiFi.begin(CAMERA_SSID,CAMERA_PASSWORD);
  }
  delay(1);
}