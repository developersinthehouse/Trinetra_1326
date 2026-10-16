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
long frontDistance = 150;
unsigned long lastSensorAt = 0, lastTelemetryAt = 0, lastCommandAt = 0;
String rxLine = "";

void setRelay(bool on) {
  relayState = on;
  digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? (on ? LOW : HIGH) : (on ? HIGH : LOW));
  Serial.print(F("[UNO DEBUG] setRelay -> "));
  Serial.println(on ? F("RELAY_ON") : F("RELAY_OFF"));
}

void setSpeedAll() {
  motor1.setSpeed(MOTOR_SPEED);
  motor2.setSpeed(MOTOR_SPEED);
  motor3.setSpeed(MOTOR_SPEED);
  motor4.setSpeed(MOTOR_SPEED);
}

void stopMotors() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void drive(char cmd) {
  setSpeedAll();
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
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 15000UL);
  if (!duration) return 999;
  long cm = duration / 58;
  return (cm < 2 || cm > 400) ? 999 : cm;
}

void handleMove(char cmd) {
  lastCommandAt = millis();
  if (cmd == 'S') {
    stopMotors();
    currentCommand = 'S';
    Serial.println(F("MOVE:S"));
    return;
  }
  
  // Agar distance 999 hai (yani sensor read nahi kar raha), toh safety ke liye gadi ko block mat karo, chalne do
  if (cmd == 'F' && frontDistance > 0 && frontDistance <= SAFE_DISTANCE_CM) {
    stopMotors();
    currentCommand = 'S';
    Serial.println(F("OBSTACLE_BLOCK_FORWARD"));
    return;
  }
  
  currentCommand = cmd;
  drive(cmd);
  Serial.print(F("MOVE:"));
  Serial.println(cmd);
}

void processLine(String line) {
  line.trim();
  if (!line.length()) return;

  if (line == "RELAY:1") { setRelay(true); return; }
  if (line == "RELAY:0") { setRelay(false); return; }
  if (line == "UNLOCK") {
    targetLocked = false;
    Serial.println(F("TARGET_UNLOCKED"));
    return;
  }
  if (line.startsWith("LOCK:")) {
    int a = line.substring(5).toInt();
    if (a >= 0 && a <= 180) {
      lockedAngle = a;
      targetLocked = true;
      targetServo.write(lockedAngle);
      Serial.print(F("TARGET_LOCKED:"));
      Serial.println(lockedAngle);
    }
    return;
  }
  if (line.length() == 1) {
    char c = line.charAt(0);
    if (c == 'F' || c == 'B' || c == 'L' || c == 'R' || c == 'S') {
      handleMove(c);
    }
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
  Serial.begin(SERIAL_BAUD);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);
  
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? HIGH : LOW);
  
  targetServo.targetServo.attach(TARGET_SERVO_PIN); // Yahan targetServo.attach(TARGET_SERVO_PIN) rakhein
  targetServo.write(90);
  
  setRelay(false);
  stopMotors();
  
  Serial.println(F("[UNO BOOT] TRINETRA UNO READY"));
}

void loop() {
  readSerialCommands();
  
  if (millis() - lastSensorAt >= 100UL) {
    lastSensorAt = millis();
    frontDistance = readFrontDistance();
  }

  if (currentCommand != 'S' && millis() - lastCommandAt > COMMAND_TIMEOUT_MS) {
    stopMotors();
    currentCommand = 'S';
  }

  if (millis() - lastTelemetryAt >= 250UL) {
    lastTelemetryAt = millis();
    Serial.print(F("DIST:"));
    Serial.println(frontDistance);
  }
}