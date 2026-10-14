#include "AudioControl.h"
#include "Config.h"

// =====================================================
// TRINETRA - AUDIO CONTROL
// =====================================================

static bool audioPlaying = false;


// =====================================================
// Initialize
// =====================================================

void audioBegin()
{
    // Audio output hardware will be initialized
    // when the selected audio storage/output module
    // is added.

    audioPlaying = false;

    Serial.println("AUDIO SYSTEM READY");
}


// =====================================================
// Demo Audio
// =====================================================

void playDemoAudio()
{
    audioPlaying = true;

    Serial.println("DEMO AUDIO: PLAY");

    // Actual audio playback will be connected here.
    //
    // Demo:
    // TRINETRA introduction
}


// =====================================================
// Horn Audio
// =====================================================

void playHornAudio()
{
    audioPlaying = true;

    Serial.println("HORN AUDIO: PLAY");

    // Actual horn audio playback
    // will be connected here.
}


// =====================================================
// Stop Audio
// =====================================================

void stopAudio()
{
    audioPlaying = false;

    Serial.println("AUDIO: STOP");
}