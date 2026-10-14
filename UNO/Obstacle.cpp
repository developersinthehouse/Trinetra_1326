#include "Obstacle.h"
#include "Config.h"

// =====================================================
// TRINETRA - OBSTACLE DETECTION
// HC-SR04 Ultrasonic Sensor
// =====================================================

static long lastDistance = 999;


// =====================================================
// Initialize Ultrasonic Sensor
// =====================================================

void obstacleBegin()
{
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    digitalWrite(TRIG_PIN, LOW);

    lastDistance = 999;
}


// =====================================================
// Read Distance
// =====================================================

long getObstacleDistance()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);

    digitalWrite(TRIG_PIN, LOW);

    unsigned long duration = pulseIn(
        ECHO_PIN,
        HIGH,
        ECHO_TIMEOUT
    );

    // No echo received
    if (duration == 0)
    {
        lastDistance = 999;
        return lastDistance;
    }

    long distance = duration * 0.0343 / 2;

    // Ignore impossible readings
    if (distance <= 0 || distance > 400)
    {
        lastDistance = 999;
        return lastDistance;
    }

    lastDistance = distance;

    return lastDistance;
}


// =====================================================
// Check Obstacle
// =====================================================

bool isObstacleDetected()
{
    long distance = getObstacleDistance();

    if (distance <= SAFE_DISTANCE)
    {
        return true;
    }

    return false;
}


// =====================================================
// Get Last Distance
// =====================================================

long getLastDistance()
{
    return lastDistance;
}