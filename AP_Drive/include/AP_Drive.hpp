#ifndef AP_DRIVE_H
#define AP_DRIVE_H

#include "AudioIO.hpp"

class AP_Drive {
public:
    AP_Drive(AudioIO& audio, float volume = 1.0f, float gain = 1.0f, float drive = 0.0f);

    void processAudio();
    void setVolume(float volume);
    void setGain(float gain);
    void setDrive(float drive);

private:
    AudioIO& audio;
    float volume;
    float gain;
    float drive;

    float applyGain(float sample, float gainLevel);
    float applyDrive(float sample, float driveLevel);
};

#endif // AP_DRIVE_H
