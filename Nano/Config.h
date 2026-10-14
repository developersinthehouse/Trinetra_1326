#ifndef TRINETRA_NANO_CONFIG_H
#define TRINETRA_NANO_CONFIG_H

// HC-SR04
#define TRIG_PIN 7
#define ECHO_PIN 8

// Radar SG90
#define SERVO_PIN 9

// 0 -> 180 -> 0
#define RADAR_MIN_ANGLE 0
#define RADAR_MAX_ANGLE 180
#define RADAR_STEP 2

// Radar reports up to this distance.
// ESP32 decides target detection at 150 cm.
#define MAX_RADAR_DISTANCE 249
#define MAX_DETECTION_DISTANCE 249

// Nano <-> ESP32 UART
#define SERIAL_BAUD 9600

// Servo movement delay
#define SCAN_DELAY 25

#endif
