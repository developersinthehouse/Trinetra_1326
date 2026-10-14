#ifndef TRINETRA_RADAR_H
#define TRINETRA_RADAR_H

#include <Arduino.h>

// Initialize radar sensor
void radarBegin();

// Take one distance measurement
long readRadarDistance();

// Send radar data
void sendRadarData(int angle, long distance);

#endif