#ifndef TRINETRA_ESP32_CONFIG_H
#define TRINETRA_ESP32_CONFIG_H

// =====================================================
// TRINETRA - ESP32 CONFIGURATION
// =====================================================


// =====================================================
// Wi-Fi
// =====================================================

#define WIFI_SSID "Airtel_rame_9730"
#define WIFI_PASSWORD "AdityaMishravlogsOfficial136"


// =====================================================
// Serial Communication
// Nano <-> ESP32
// =====================================================

#define ESP32_BAUD 115200

// Nano TX -> ESP32 RX
#define NANO_RX_PIN 16

// ESP32 TX -> Nano RX
#define NANO_TX_PIN 17


// =====================================================
// Web Server
// =====================================================

#define SERVER_PORT 80


// =====================================================
// Buzzer / Alert
// =====================================================

// Fire/alert demonstration buzzer
#define BUZZER_PIN 26


// =====================================================
// Relay Control (Safe Low-Voltage Test Load)
// =====================================================
#define RELAY_PIN 27

// =====================================================
// Audio Control
// =====================================================

// Demo recorded-audio control (remapped from 27 to avoid relay collision)
#define AUDIO_DEMO_PIN 25

// Horn recorded-audio control
#define AUDIO_HORN_PIN 14


// =====================================================
// SECOND SERVO
// Target Direction Indicator
// =====================================================

// Servo 1 = Nano radar servo
// Servo 2 = ESP32 direction indicator servo

#define LOCK_SERVO_PIN 13


// =====================================================
// Radar Settings
// =====================================================

#define RADAR_MIN_ANGLE 0
#define RADAR_MAX_ANGLE 180

#define MAX_RADAR_DISTANCE 300


// =====================================================
// Target Lock
// =====================================================

// Target is considered detected within this distance
#define TARGET_DISTANCE_CM 100

// Minimum time between target updates
#define RADAR_UPDATE_INTERVAL 50


#endif