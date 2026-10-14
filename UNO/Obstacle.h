#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <Arduino.h>

// Initialize ultrasonic sensor
void obstacleBegin();

// Read distance in centimeters
long getObstacleDistance();

// Check whether an obstacle is inside the safe distance
bool isObstacleDetected();

// Get the latest measured distance
long getLastDistance();

#endif