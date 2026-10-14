#ifndef TRINETRA_SERVO_CONTROL_H
#define TRINETRA_SERVO_CONTROL_H

#include <Arduino.h>

void servoBegin();

void moveServo(
    int angle
);

int getServoAngle();

#endif