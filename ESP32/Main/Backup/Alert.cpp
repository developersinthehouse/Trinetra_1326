#include "Alert.h"
#include "Config.h"

// =====================================================
// TRINETRA - BUZZER ALERT
// =====================================================

static bool alertState = false;


// =====================================================
// Initialize
// =====================================================

void alertBegin()
{
    pinMode(BUZZER_PIN, OUTPUT);

    digitalWrite(BUZZER_PIN, LOW);

    alertState = false;
}


// =====================================================
// Alert ON
// =====================================================

void alertOn()
{
    digitalWrite(BUZZER_PIN, HIGH);

    alertState = true;

    Serial.println("ALERT BUZZER: ON");
}


// =====================================================
// Alert OFF
// =====================================================

void alertOff()
{
    digitalWrite(BUZZER_PIN, LOW);

    alertState = false;

    Serial.println("ALERT BUZZER: OFF");
}


// =====================================================
// Toggle
// =====================================================

void alertToggle()
{
    if (alertState)
    {
        alertOff();
    }
    else
    {
        alertOn();
    }
}


// =====================================================
// Get State
// =====================================================

bool isAlertOn()
{
    return alertState;
}