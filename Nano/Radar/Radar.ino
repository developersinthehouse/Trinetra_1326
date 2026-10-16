#include <Arduino.h>
#include <Servo.h>
#include <SoftwareSerial.h>

#define TRIG_PIN 7
#define ECHO_PIN 8
#define SERVO_PIN 9

// Nano पर SoftwareSerial के लिए Pins (आप इन्हें बदल भी सकते हैं)
#define NANO_RX_PIN 2
#define NANO_TX_PIN 3

#define MIN_ANGLE 0
#define MAX_ANGLE 180
#define STEP_ANGLE 2

#define MAX_DISTANCE 249
#define SERVO_DELAY 30

Servo radarServo;
SoftwareSerial espSerial(NANO_RX_PIN, NANO_TX_PIN); // ESP32 से बात करने के लिए

int angleNow = 0;
int direction = 1;

bool locked = false;
int lockedAngle = 0;
bool radarRunning = true;

String commandBuffer;

// =====================================================
// ULTRASONIC
// =====================================================
long readDistance()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL);

  if (duration == 0)
    return MAX_DISTANCE;

  long distance = duration / 58;

  if (distance < 2 || distance > MAX_DISTANCE)
    return MAX_DISTANCE;

  return distance;
}

// =====================================================
// RADAR DATA (ESP32 को भेजना)
// =====================================================
void sendRadar()
{
  long distance = readDistance();

  // ESP32 को डेटा भेजना (SoftwareSerial के ज़रिए)
  espSerial.print("RADAR,");
  espSerial.print(angleNow);
  espSerial.print(",");
  espSerial.println(distance);
}

// =====================================================
// LOCK
// =====================================================
void lockRadar(int angle)
{
  angle = constrain(angle, 0, 180);

  lockedAngle = angle;
  angleNow = angle;
  locked = true;

  radarServo.write(lockedAngle);

  Serial.println(F("========================================"));
  Serial.print(F("[NANO DEBUG] Radar Locked at: "));
  Serial.println(lockedAngle);
  Serial.println(F("========================================"));

  espSerial.print("RADAR_LOCKED,");
  espSerial.println(lockedAngle);
}

// =====================================================
// UNLOCK
// =====================================================
void unlockRadar()
{
  locked = false;

  if (angleNow >= MAX_ANGLE)
    direction = -1;
  else if (angleNow <= MIN_ANGLE)
    direction = 1;

  Serial.println(F("========================================"));
  Serial.println(F("[NANO DEBUG] Radar Unlocked"));
  Serial.println(F("========================================"));

  espSerial.println("RADAR_UNLOCKED");
}

// =====================================================
// COMMAND PROCESSOR
// =====================================================
void processCommand(String command)
{
  command.trim();
  if (command.length() == 0)
    return;

  Serial.println(F("----------------------------------------"));
  Serial.print(F("[NANO RECEIVED ← ESP32]: "));
  Serial.println(command);
  Serial.println(F("----------------------------------------"));

  if (command.startsWith("LOCK:"))
  {
    int angle = command.substring(5).toInt();
    lockRadar(angle);
    return;
  }

  if (command == "UNLOCK")
  {
    unlockRadar();
    return;
  }

  if (command == "STOP")
  {
    radarRunning = false;
    locked = false;
    Serial.println(F("[NANO DEBUG] Radar Stopped"));
    espSerial.println("RADAR_STOPPED");
    return;
  }

  if (command == "START")
  {
    radarRunning = true;
    locked = false;
    Serial.println(F("[NANO DEBUG] Radar Started"));
    espSerial.println("RADAR_STARTED");
    return;
  }
}

// =====================================================
// READ COMMAND FROM ESP32
// =====================================================
void readCommands()
{
  while (espSerial.available())
  {
    char c = espSerial.read();

    if (c == '\n')
    {
      processCommand(commandBuffer);
      commandBuffer = "";
    }
    else if (c != '\r')
    {
      commandBuffer += c;
      if (commandBuffer.length() > 32)
        commandBuffer = "";
    }
  }
}

// =====================================================
// SETUP
// =====================================================
void setup()
{
  Serial.begin(115200);      // कंप्यूटर पर मॉनिटर देखने के लिए (115200)
  espSerial.begin(9600);    // ESP32 के साथ कम्यूनिकेशन के लिए (9600)

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  radarServo.attach(SERVO_PIN);

  angleNow = 0;
  direction = 1;
  radarServo.write(0);

  delay(500);

  Serial.println(F("========================================"));
  Serial.println(F("[BOOT] NANO RADAR READY (SoftwareSerial)"));
  Serial.println(F("========================================"));
}

// =====================================================
// LOOP
// =====================================================
void loop()
{
  readCommands();

  if (!radarRunning)
  {
    delay(50);
    return;
  }

  if (locked)
  {
    radarServo.write(lockedAngle);
    angleNow = lockedAngle;
    sendRadar();
    delay(100);
    return;
  }

  radarServo.write(angleNow);
  delay(SERVO_DELAY);

  readCommands();

  if (locked)
    return;

  sendRadar();

  angleNow += direction * STEP_ANGLE;

  if (angleNow >= MAX_ANGLE)
  {
    angleNow = MAX_ANGLE;
    direction = -1;
  }

  if (angleNow <= MIN_ANGLE)
  {
    angleNow = MIN_ANGLE;
    direction = 1;
  }
}