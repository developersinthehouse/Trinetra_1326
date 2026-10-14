#ifndef TRINETRA_RADAR_DATA_H
#define TRINETRA_RADAR_DATA_H

#include <Arduino.h>

// =====================================================
// TRINETRA - RADAR DATA
// =====================================================

// Initialize communication with Nano
void radarDataBegin();

// Read incoming radar data from Nano
void radarDataUpdate();

// Check whether new radar data is available
bool radarDataAvailable();

// Get latest radar angle
int getRadarAngle();

// Get latest radar distance
long getRadarDistance();

// Check whether a target is detected
bool isTargetDetected();

// Get latest target angle
int getTargetAngle();

// Get latest target distance
long getTargetDistance();

#endif