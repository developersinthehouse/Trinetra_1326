#include <Arduino.h>
#include <AFMotor.h>
#include <Servo.h>

#define TRIG_PIN A0
#define ECHO_PIN A1
#define RELAY_PIN 2
#define TARGET_SERVO_PIN 9

#define SAFE_DISTANCE_CM 10
#define MOTOR_SPEED 220
#define SERIAL_BAUD 9600
#define COMMAND_TIMEOUT_MS 1500UL

const bool RELAY_ACTIVE_LOW = true;
AF_DCMotor motor1(1), motor2(2), motor3(3), motor4(4);
Servo targetServo;

char currentCommand = 'S';
bool relayState = false;
bool targetLocked = false;
int lockedAngle = 90;
long frontDistance = 999;
unsigned long lastSensorAt = 0, lastTelemetryAt = 0, lastCommandAt = 0;
String rxLine;

void setRelay(bool on) {
  relayState = on;
  digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? (on ? LOW : HIGH) : (on ? HIGH : LOW));
  
  // Debug prints
  Serial.print(F("[DEBUG] setRelay called. State -> "));
  Serial.println(on ? F("RELAY_ON") : F("RELAY_OFF"));
}

void setSpeedAll() {
  motor1.setSpeed(MOTOR_SPEED);
  motor2.setSpeed(MOTOR_SPEED);
  motor3.setSpeed(MOTOR_SPEED);
  motor4.setSpeed(MOTOR_SPEED);
}

void stopMotors() {
  Serial.println(F("[DEBUG] stopMotors() called. Stopping all motors."));
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void drive(char cmd) {
  setSpeedAll();
  Serial.print(F("[DEBUG] drive() executing command: "));
  Serial.println(cmd);
  
  switch (cmd) {
    case 'F':
      motor1.run(FORWARD); motor2.run(FORWARD); motor3.run(FORWARD); motor4.run(FORWARD);
      break;
    case 'B':
      motor1.run(BACKWARD); motor2.run(BACKWARD); motor3.run(BACKWARD); motor4.run(BACKWARD);
      break;
    case 'L':
      motor1.run(BACKWARD); motor2.run(BACKWARD); motor3.run(FORWARD); motor4.run(FORWARD);
      break;
    case 'R':
      motor1.run(FORWARD); motor2.run(FORWARD); motor3.run(BACKWARD); motor4.run(BACKWARD);
      break;
    default:
      stopMotors();
      break;
  }
}

long readFrontDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 10000UL);
  if (!duration) return 999;
  long cm = duration / 58;
  return (cm < 2 || cm > 400) ? 999 : cm;
}

void handleMove(char cmd) {
  Serial.print(F("[DEBUG] handleMove received command: "));
  Serial.println(cmd);
  
  lastCommandAt = millis();
  if (cmd == 'S') {
    stopMotors();
    currentCommand = 'S';
    Serial.println(F("[DEBUG] Motor stopped via 'S' command. Sending MOVE:S."));
    Serial.println(F("MOVE:S"));
    return;
  }
  
  // Sirf FORWARD ke liye obstacle safety check lagaya hai, taaki B, L, R me rukawat na aaye
  if (cmd == 'F' && frontDistance <= SAFE_DISTANCE_CM) {
    stopMotors();
    currentCommand = 'S';
    Serial.print(F("[DEBUG] Obstacle detected! Front distance ("));
    Serial.print(frontDistance);
    Serial.println(F(" cm) is <= SAFE_DISTANCE_CM. Blocking FORWARD."));
    Serial.println(F("OBSTACLE_BLOCK_FORWARD"));
    return;
  }
  
  currentCommand = cmd;
  drive(cmd);
  Serial.print(F("[DEBUG] Motor movement command executed successfully: "));
  Serial.println(cmd);
  
  Serial.print(F("MOVE:"));
  Serial.println(cmd);
}

void processLine(String line) {
  line.trim();
  if (!line.length()) return;

  Serial.print(F("[DEBUG] Processing incoming line from RX: ["));
  Serial.print(line);
  Serial.println(F("]"));

  if (line == "RELAY:1") { 
    Serial.println(F("[DEBUG] Command matched: RELAY:1"));
    setRelay(true); 
    return; 
  }
  if (line == "RELAY:0") { 
    Serial.println(F("[DEBUG] Command matched: RELAY:0"));
    setRelay(false); 
    return; 
  }
  if (line == "UNLOCK") {
    Serial.println(F("[DEBUG] Command matched: UNLOCK"));
    targetLocked = false;
    Serial.println(F("TARGET_UNLOCKED"));
    return;
  }
  if (line.startsWith("LOCK:")) {
    Serial.println(F("[DEBUG] Command matched starting with LOCK:"));
    int a = line.substring(5).toInt();
    if (a >= 0 && a <= 180) {
      lockedAngle = a;
      targetLocked = true;
      targetServo.write(lockedAngle);
      Serial.print(F("[DEBUG] Target servo locked at angle: "));
      Serial.println(lockedAngle);
      Serial.print(F("TARGET_LOCKED:"));
      Serial.println(lockedAngle);
    } else {
      Serial.print(F("[DEBUG] Invalid LOCK angle received: "));
      Serial.println(a);
    }
    return;
  }
  if (line.length() == 1) {
    char c = line.charAt(0);
    if (c == 'F' || c == 'B' || c == 'L' || c == 'R' || c == 'S') {
      Serial.print(F("[DEBUG] Single-char movement command recognized: "));
      Serial.println(c);
      handleMove(c);
    } else {
      Serial.print(F("[DEBUG] Unrecognized single character command: "));
      Serial.println(c);
    }
  } else {
    Serial.print(F("[DEBUG] Unrecognized multi-character command line: "));
    Serial.println(line);
  }
}

void readSerialCommands() {
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\n') {
      processLine(rxLine);
      rxLine = "";
    } else if (c != '\r' && rxLine.length() < 48) {
      rxLine += c;
    }
  }
}

void setup() {
  Serial.begin(SERIAL_BAUD); // Yeh Uno ke default RX (Pin 0) aur TX (Pin 1) ko use karega
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);
  
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? HIGH : LOW);
  
  targetServo.attach(TARGET_SERVO_PIN);
  targetServo.write(90);
  
  setRelay(false);
  stopMotors();
  
  Serial.println(F("[DEBUG] Setup complete. TRINETRA UNO READY"));
}

void loop() {
  readSerialCommands();
  
  if (millis() - lastSensorAt >= 100UL) {
    lastSensorAt = millis();
    frontDistance = readFrontDistance();
  }

  if (currentCommand != 'S' && millis() - lastCommandAt > COMMAND_TIMEOUT_MS) {
    Serial.print(F("[DEBUG] Command timeout reached ("));
    Serial.print(COMMAND_TIMEOUT_MS);
    Serial.println(F(" ms). Stopping motors automatically."));
    stopMotors();
    currentCommand = 'S';
  }

  if (millis() - lastTelemetryAt >= 250UL) {
    lastTelemetryAt = millis();
    Serial.print(F("DIST:"));
    Serial.println(frontDistance);
  }
}