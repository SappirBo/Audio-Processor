#ifndef AP_DRIVE_H
#define AP_DRIVE_H

#include "AudioIO.hpp"
#include <iostream>

class AP_Drive {
public:
    AP_Drive(AudioIO& audio,int16_t level = 0, int16_t drive = 0);

    void processAudio();

private:
    AudioIO& m_audio;
    float m_drive;
    int16_t m_level;
    
    void setDrive(int16_t drive);
    void setLevel(int16_t level);

    int16_t applyDrive(int16_t sample);
    int16_t applyLevel(int16_t sample);
};

#endif // AP_DRIVE_H
