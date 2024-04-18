#ifndef AP_DRIVE_H
#define AP_DRIVE_H

#include "AudioIO.hpp"
#include <iostream>

class AP_Drive {
public:
    AP_Drive(AudioIO& audio, int16_t drive = 0);

    void processAudio();
    void setDrive(int16_t drive);

private:
    AudioIO& m_audio;
    float m_drive;

    // int16_t applyGain (int16_t sample);
    int16_t applyDrive(int16_t sample);
};

#endif // AP_DRIVE_H
