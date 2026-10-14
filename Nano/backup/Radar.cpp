#include "Radar.h"
#include "Config.h"

void radarBegin()
{
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    digitalWrite(TRIG_PIN, LOW);
}

long readRadarDistance()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);

    digitalWrite(TRIG_PIN, LOW);

    unsigned long duration =
        pulseIn(ECHO_PIN, HIGH, 30000UL);

    if (duration == 0)
    {
        return MAX_DETECTION_DISTANCE;
    }

    long distance =
        duration * 0.0343 / 2.0;

    if (distance < 2)
    {
        return MAX_DETECTION_DISTANCE;
    }

    if (distance > MAX_DETECTION_DISTANCE)
    {
        return MAX_DETECTION_DISTANCE;
    }

    return distance;
}

void sendRadarData(
    int angle,
    long distance
)
{
    Serial.print("RADAR,");
    Serial.print(angle);
    Serial.print(",");
    Serial.println(distance);
}