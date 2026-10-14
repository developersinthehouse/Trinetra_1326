#ifndef TRINETRA_SERVO_CONTROL_H
#define TRINETRA_SERVO_CONTROL_H

#include <Arduino.h>

// Initialize servo
void servoBegin();

// Move servo to a specific angle
void moveServo(int angle);

// Get current servo angle
int getServoAngle();

#endif