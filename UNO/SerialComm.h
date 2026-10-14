#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include <Arduino.h>

// Initialize serial communication
void serialCommBegin();

// Check whether a command is available
bool commandAvailable();

// Read one command from serial
char readCommand();

// Send a simple status message
void sendStatus(const char* message);

// Send distance/obstacle telemetry
void sendObstacleStatus(long distance, bool obstacle);

#endif