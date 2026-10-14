#include <Arduino.h>
#include "Obstacle.h"
#include "Config.h"

void obstacleBegin() {
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

long getObstacleDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT);
    if (duration == 0) return 999;
    
    long distance = duration * 0.034 / 2;
    return distance;
}

bool isObstacleDetected() {
    long dist = getObstacleDistance();
    return (dist > 0 && dist <= SAFE_DISTANCE);
}