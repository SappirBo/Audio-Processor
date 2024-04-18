#ifndef AP_DRIVE_H
#define AP_DRIVE_H

#include "AudioIO.hpp"
#include <iostream>

class AP_Drive {
public:
    AP_Drive(AudioIO& audio, int16_t volume = 1, int16_t gain = 1, int16_t drive = 0);

    void processAudio();
    void setVolume(int16_t volume);
    void setGain(int16_t gain);
    void setDrive(int16_t drive);

private:
    AudioIO& m_audio;
    int16_t m_volume;
    int16_t m_gain;
    int16_t m_drive;

    int16_t applyGain (int16_t sample);
    int16_t applyDrive(int16_t sample);
};

#endif // AP_DRIVE_H
