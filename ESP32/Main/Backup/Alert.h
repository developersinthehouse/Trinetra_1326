#ifndef TRINETRA_ALERT_H
#define TRINETRA_ALERT_H

#include <Arduino.h>

// Initialize buzzer
void alertBegin();

// Turn alert buzzer ON
void alertOn();

// Turn alert buzzer OFF
void alertOff();

// Toggle buzzer
void alertToggle();

// Get current alert state
bool isAlertOn();

#endif