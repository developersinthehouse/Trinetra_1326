#include "ServoControl.h"
#include "Config.h"
#include <Servo.h>

// =====================================================
// TRINETRA - SERVO CONTROL
// =====================================================

Servo radarServo;

static int currentAngle = 90;

// -----------------------------------------------------
// Initialize Servo
// -----------------------------------------------------

void servoBegin()
{
    radarServo.attach(SERVO_PIN);

    currentAngle = 90;

    radarServo.write(currentAngle);

    delay(500);
}

// -----------------------------------------------------
// Move Servo
// -----------------------------------------------------

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

// -----------------------------------------------------
// Get Current Angle
// -----------------------------------------------------

int getServoAngle()
{
    return currentAngle;
}