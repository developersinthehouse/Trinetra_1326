#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

const int ENA = 5;
const int IN1 = 6;
const int IN2 = 7;
const int IN3 = 8;
const int IN4 = 9;
const int ENB = 10;

const int TRIG_PIN = A0;
const int ECHO_PIN = A1;

const long SAFE_DISTANCE = 25;
const unsigned long ECHO_TIMEOUT = 30000L;
const int MOTOR_SPEED = 200;
const long SERIAL_BAUD = 9600;

// Command Characters
const char CMD_FORWARD  = 'F';
const char CMD_BACKWARD = 'B';
const char CMD_LEFT     = 'L';
const char CMD_RIGHT    = 'R';
const char CMD_STOP     = 'S';

#endif