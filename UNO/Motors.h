#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>

// Initialize all motors
void motorsBegin();

// Set motor speed
void setMotorSpeed(int speed);

// Movement functions
void moveForward();
void moveBackward();

void pivotLeft();
void pivotRight();

void stopMotors();

#endif