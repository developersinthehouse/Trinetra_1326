#include "RadarData.h"
#include "Config.h"

// =====================================================
// TRINETRA - RADAR DATA RECEIVER
// ESP32 <-> NANO
// =====================================================

HardwareSerial NanoSerial(2);

static String incomingData = "";

static int latestAngle = 0;
static long latestDistance = 999;

static int targetAngle = 0;
static long targetDistance = 999;

static bool newRadarData = false;
static bool targetDetected = false;


// =====================================================
// Initialize Nano Serial
// =====================================================

void radarDataBegin()
{
    NanoSerial.begin(
        ESP32_BAUD,
        SERIAL_8N1,
        NANO_RX_PIN,
        NANO_TX_PIN
    );

    incomingData.reserve(64);

    latestAngle = 0;
    latestDistance = 999;

    targetAngle = 0;
    targetDistance = 999;

    newRadarData = false;
    targetDetected = false;
}


// =====================================================
// Process One Radar Line
// Format:
//
// RADAR,ANGLE,DISTANCE
//
// Example:
// RADAR,74,25
// =====================================================

static void processRadarLine(String line)
{
    line.trim();

    if (!line.startsWith("RADAR,"))
    {
        return;
    }

    int firstComma = line.indexOf(',');

    if (firstComma < 0)
    {
        return;
    }

    int secondComma = line.indexOf(',', firstComma + 1);

    if (secondComma < 0)
    {
        return;
    }

    String angleText =
        line.substring(firstComma + 1, secondComma);

    String distanceText =
        line.substring(secondComma + 1);

    int angle = angleText.toInt();
    long distance = distanceText.toInt();

    // Basic validation
    if (angle < 0 || angle > 180)
    {
        return;
    }

    if (distance < 0 || distance > 400)
    {
        return;
    }

    latestAngle = angle;
    latestDistance = distance;

    newRadarData = true;


    // =================================================
    // Target Detection
    // =================================================

    if (distance <= TARGET_DISTANCE_CM)
    {
        targetDetected = true;

        targetAngle = angle;
        targetDistance = distance;
    }
}


// =====================================================
// Read Radar Data
// =====================================================

void radarDataUpdate()
{
    while (NanoSerial.available())
    {
        char c = NanoSerial.read();

        // End of line
        if (c == '\n')
        {
            processRadarLine(incomingData);

            incomingData = "";
        }

        // Ignore carriage return
        else if (c == '\r')
        {
            continue;
        }

        // Store character
        else
        {
            incomingData += c;

            // Prevent oversized strings
            if (incomingData.length() > 80)
            {
                incomingData = "";
            }
        }
    }
}


// =====================================================
// New Radar Data Available
// =====================================================

bool radarDataAvailable()
{
    if (newRadarData)
    {
        newRadarData = false;

        return true;
    }

    return false;
}


// =====================================================
// Latest Radar Angle
// =====================================================

int getRadarAngle()
{
    return latestAngle;
}


// =====================================================
// Latest Radar Distance
// =====================================================

long getRadarDistance()
{
    return latestDistance;
}


// =====================================================
// Target Detected
// =====================================================

bool isTargetDetected()
{
    return targetDetected;
}


// =====================================================
// Target Angle
// =====================================================

int getTargetAngle()
{
    return targetAngle;
}


// =====================================================
// Target Distance
// =====================================================

long getTargetDistance()
{
    return targetDistance;
}