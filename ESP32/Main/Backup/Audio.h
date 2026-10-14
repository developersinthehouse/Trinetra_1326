#ifndef AUDIO_H
#define AUDIO_H

#include <Arduino.h>
#include <AudioFileSourcePROGMEM.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>

class AudioSystem {
public:
    AudioSystem();
    void begin();
    void playIntroduction();
    void playHorn();
    void loop();

private:
    AudioOutputI2S *out;
    AudioGeneratorMP3 *mp3;
    AudioFileSourcePROGMEM *file;
};

// Global instance declaration
extern AudioSystem audioSys;

#endif