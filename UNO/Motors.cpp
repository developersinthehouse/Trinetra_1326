#include "Motors.h"
#include "Config.h"
#include <AFMotor.h>

// =====================================================
// TRINETRA - MOTOR CONTROL
// L293D Motor Shield V1
// =====================================================

// Motor objects
AF_DCMotor M1(1);
AF_DCMotor M2(2);
AF_DCMotor M3(3);
AF_DCMotor M4(4);

// Current speed
static int currentSpeed = MOTOR_SPEED;


// =====================================================
// Initialize Motors
// =====================================================

void motorsBegin()
{
    M1.setSpeed(currentSpeed);
    M2.setSpeed(currentSpeed);
    M3.setSpeed(currentSpeed);
    M4.setSpeed(currentSpeed);

    stopMotors();
}


// =====================================================
// Set Motor Speed
// =====================================================

void setMotorSpeed(int speed)
{
    speed = constrain(speed, 0, 255);

    currentSpeed = speed;

    M1.setSpeed(currentSpeed);
    M2.setSpeed(currentSpeed);
    M3.setSpeed(currentSpeed);
    M4.setSpeed(currentSpeed);
}


// =====================================================
// FORWARD
// =====================================================

void moveForward()
{
    M1.setSpeed(currentSpeed);
    M2.setSpeed(currentSpeed);
    M3.setSpeed(currentSpeed);
    M4.setSpeed(currentSpeed);

    M1.run(FORWARD);
    M2.run(FORWARD);
    M3.run(FORWARD);
    M4.run(FORWARD);
}


// =====================================================
// BACKWARD
// =====================================================

void moveBackward()
{
    M1.setSpeed(currentSpeed);
    M2.setSpeed(currentSpeed);
    M3.setSpeed(currentSpeed);
    M4.setSpeed(currentSpeed);

    M1.run(BACKWARD);
    M2.run(BACKWARD);
    M3.run(BACKWARD);
    M4.run(BACKWARD);
}


// =====================================================
// LEFT PIVOT
// =====================================================

void pivotLeft()
{
    M1.run(BACKWARD);
    M2.run(BACKWARD);

    M3.run(FORWARD);
    M4.run(FORWARD);
}


// =====================================================
// RIGHT PIVOT
// =====================================================

void pivotRight()
{
    M1.run(FORWARD);
    M2.run(FORWARD);

    M3.run(BACKWARD);
    M4.run(BACKWARD);
}


// =====================================================
// STOP
// =====================================================

void stopMotors()
{
    M1.run(RELEASE);
    M2.run(RELEASE);
    M3.run(RELEASE);
    M4.run(RELEASE);
}