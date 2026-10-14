#ifndef TRINETRA_AUDIO_CONTROL_H
#define TRINETRA_AUDIO_CONTROL_H

#include <Arduino.h>

// Initialize audio control system
void audioBegin();

// Play TRINETRA introduction
void playDemoAudio();

// Play horn sound
void playHornAudio();

// Stop currently playing audio
void stopAudio();

#endif