#include "SerialComm.h"
#include "Config.h"

// =====================================================
// TRINETRA - SERIAL COMMUNICATION
// UNO <-> ESP32
// =====================================================

// -----------------------------------------------------
// Initialize Serial
// -----------------------------------------------------

void serialCommBegin()
{
    Serial.begin(SERIAL_BAUD);
}


// -----------------------------------------------------
// Check for Command
// -----------------------------------------------------

bool commandAvailable()
{
    return Serial.available() > 0;
}


// -----------------------------------------------------
// Read Command
// -----------------------------------------------------

char readCommand()
{
    while (Serial.available() > 0)
    {
        char command = Serial.read();

        // Ignore newline and carriage return
        if (command == '\n' || command == '\r')
        {
            continue;
        }

        return command;
    }

    return '\0';
}


// -----------------------------------------------------
// Send Status Message
// -----------------------------------------------------

void sendStatus(const char* message)
{
    Serial.println(message);
}


// -----------------------------------------------------
// Send Obstacle Status
// -----------------------------------------------------

void sendObstacleStatus(long distance, bool obstacle)
{
    Serial.print("DIST:");
    Serial.print(distance);

    Serial.print(",OBS:");

    if (obstacle)
    {
        Serial.println("1");
    }
    else
    {
        Serial.println("0");
    }
}