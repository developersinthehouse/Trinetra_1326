#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include <Arduino.h>

void serialCommBegin();
bool commandAvailable(String &cmdOut);

#endif