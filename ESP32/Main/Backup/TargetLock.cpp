#include "TargetLock.h"
#include "IndicatorServo.h"

extern bool targetLocked;
extern bool targetDetected;
extern int detectedAngle;
extern long detectedDistance;
extern int lockedAngle;
extern long lockedDistance;
extern HardwareSerial TankSerial;

void lockTarget()
{
  if (targetLocked)
    return;
  if (!targetDetected)
  {
    Serial.println("LOCK FAILED: NO TARGET WITHIN 150 CM");
    return;
  }

  lockedAngle = detectedAngle;
  lockedDistance = detectedDistance;
  targetLocked = true;

  setIndicatorAngle(lockedAngle);

  TankSerial.print("LOCK:");
  TankSerial.println(lockedAngle);

  Serial.print("TARGET LOCKED | ANGLE=");
  Serial.print(lockedAngle);
  Serial.print(" | DISTANCE=");
  Serial.print(lockedDistance);
  Serial.println(" cm");
}

void unlockTarget()
{
  if (!targetLocked)
    return;

  targetLocked = false;
  lockedAngle = 0;
  lockedDistance = 0;

  TankSerial.println("UNLOCK");
  setIndicatorAngle(90);

  targetDetected = false;
  detectedDistance = 0;
  Serial.println("TARGET UNLOCKED");
}