#ifndef AP_DRIVE_H
#define AP_DRIVE_H

#include "AudioIO.hpp"
#include <iostream>

class AP_Drive {
public:
    AP_Drive(AudioIO& audio, int32_t volume = 1, int32_t gain = 1, int32_t drive = 0);

    void processAudio();
    void setVolume(int32_t volume);
    void setGain(int32_t gain);
    void setDrive(int32_t drive);

private:
    AudioIO& m_audio;
    int32_t m_volume;
    int32_t m_gain;
    int32_t m_drive;

    int32_t applyGain (int32_t sample);
    int32_t applyDrive(int32_t sample);
};

#endif // AP_DRIVE_H
