#ifndef CONFIG_H
#define CONFIG_H

// =====================================================
// TRINETRA - UNO CONFIGURATION
// =====================================================

// -------------------------
// Motor Configuration
// -------------------------

#define MOTOR_SPEED 200

// -------------------------
// Ultrasonic Sensor
// -------------------------

#define TRIG_PIN A0
#define ECHO_PIN A1

// Minimum safe distance in centimeters
#define SAFE_DISTANCE 10

// Maximum time to wait for ultrasonic echo
#define ECHO_TIMEOUT 30000UL

// -------------------------
// Serial Communication
// -------------------------

#define SERIAL_BAUD 9600

// -------------------------
// Tank Commands
// -------------------------

#define CMD_FORWARD  'F'
#define CMD_BACKWARD 'B'
#define CMD_LEFT     'L'
#define CMD_RIGHT    'R'
#define CMD_STOP     'S'

// -------------------------
// Debug
// -------------------------

#define DEBUG_ENABLED true

#endif