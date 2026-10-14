#include "ServoControl.h"
#include "Config.h"
#include <Servo.h>

Servo radarServo;

static int currentAngle = 0;

void servoBegin()
{
    radarServo.attach(SERVO_PIN);

    currentAngle = RADAR_MIN_ANGLE;

    radarServo.write(currentAngle);

    delay(500);
}

void moveServo(int angle)
{
    angle = constrain(
        angle,
        RADAR_MIN_ANGLE,
        RADAR_MAX_ANGLE
    );

    radarServo.write(angle);

    currentAngle = angle;
}

int getServoAngle()
{
    return currentAngle;
}