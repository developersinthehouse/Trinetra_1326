#include <Arduino.h>
#include <Servo.h>

#define TRIG_PIN 7
#define ECHO_PIN 8
#define SERVO_PIN 9

#define MIN_ANGLE 0
#define MAX_ANGLE 180
#define STEP_ANGLE 2

#define MAX_DISTANCE 249
#define SERVO_DELAY 30

Servo radarServo;

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

  unsigned long duration =
    pulseIn(ECHO_PIN, HIGH, 30000UL);

  if (duration == 0)
    return MAX_DISTANCE;

  long distance = duration / 58;

  if (distance < 2 || distance > MAX_DISTANCE)
    return MAX_DISTANCE;

  return distance;
}


// =====================================================
// RADAR DATA
// =====================================================

void sendRadar()
{
  long distance = readDistance();

  Serial.print("RADAR,");
  Serial.print(angleNow);
  Serial.print(",");
  Serial.println(distance);
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

  Serial.print("RADAR_LOCKED,");
  Serial.println(lockedAngle);
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

  Serial.println("RADAR_UNLOCKED");
}


// =====================================================
// COMMAND
// =====================================================

void processCommand(String command)
{
  command.trim();

  if (command.length() == 0)
    return;


  Serial.print("CMD:");

  Serial.println(command);


  if (command.startsWith("LOCK:"))
  {
    int angle =
      command.substring(5).toInt();

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
    Serial.println("RADAR_STOPPED");
    return;
  }

  if (command == "START")
  {
    radarRunning = true;
    locked = false;
    Serial.println("RADAR_STARTED");
    return;
  }
}


// =====================================================
// READ ESP32 COMMAND
// =====================================================

void readCommands()
{
  while (Serial.available())
  {
    char c = Serial.read();

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
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  radarServo.attach(SERVO_PIN);

  angleNow = 0;
  direction = 1;

  radarServo.write(0);

  delay(500);

  Serial.println("NANO_RADAR_READY");
}


// =====================================================
// LOOP
// =====================================================

void loop()
{
  readCommands();

  // If radar scanning is stopped, pause sweep and measurement
  if (!radarRunning)
  {
    delay(50);
    return;
  }

  // ================================================
  // LOCKED
  // ================================================

  if (locked)
  {
    radarServo.write(lockedAngle);

    angleNow = lockedAngle;

    sendRadar();

    delay(100);

    return;
  }


  // ================================================
  // NORMAL RADAR SWEEP
  // ================================================

  radarServo.write(angleNow);

  delay(SERVO_DELAY);


  readCommands();


  if (locked)
    return;


  sendRadar();


  angleNow +=
    direction * STEP_ANGLE;


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