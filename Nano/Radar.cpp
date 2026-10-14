#include "Radar.h"
#include "Config.h"

// =====================================================
// TRINETRA - NANO RADAR
// HC-SR04 Distance Measurement
// =====================================================

static long lastDistance = 999;

// -----------------------------------------------------
// Initialize Radar
// -----------------------------------------------------

void radarBegin()
{
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    digitalWrite(TRIG_PIN, LOW);

    lastDistance = 999;
}

// -----------------------------------------------------
// Read Distance
// -----------------------------------------------------

long readRadarDistance()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(3);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);

    digitalWrite(TRIG_PIN, LOW);

    unsigned long duration =
        pulseIn(ECHO_PIN, HIGH, 30000UL);

    // No echo
    if (duration == 0)
    {
        lastDistance = 999;
        return 999;
    }

    long distance = duration * 0.0343 / 2;

    // Invalid reading
    if (distance < 2 || distance > MAX_DETECTION_DISTANCE)
    {
        lastDistance = 999;
        return 999;
    }

    lastDistance = distance;

    return distance;
}

// -----------------------------------------------------
// Send Radar Data
// Format:
// RADAR,ANGLE,DISTANCE
// -----------------------------------------------------

void sendRadarData(int angle, long distance)
{
    Serial.print("RADAR,");
    Serial.print(angle);
    Serial.print(",");
    Serial.println(distance);
}