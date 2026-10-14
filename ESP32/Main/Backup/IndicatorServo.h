#ifndef INDICATOR_SERVO_H
#define INDICATOR_SERVO_H

#include <Arduino.h>
#include <ESP32Servo.h>

extern Servo indicatorServo;
void setIndicatorAngle(int angle);

#endif