#include <Arduino.h>
#include "SerialComm.h"
#include "Config.h"

String serialBuffer = "";

void serialCommBegin() {
    Serial.begin(SERIAL_BAUD);
    serialBuffer.reserve(50);
}

bool commandAvailable(String &cmdOut) {
    while (Serial.available() > 0) {
        char c = (char)Serial.read();
        if (c == '\n') {
            cmdOut = serialBuffer;
            cmdOut.trim();
            serialBuffer = "";
            return true;
        } else if (c != '\r') {
            serialBuffer += c;
        }
    }
    return false;
}