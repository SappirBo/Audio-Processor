#ifndef AP_DRIVE_H
#define AP_DRIVE_H

#include "AudioIO.hpp"
// #include "../AP_Audio_Effect/include/AP_Audio_Effect.hpp"
#include "AP_Audio_Effect.hpp"
#include <iostream>

class AP_Drive: public AP_Audio_Effect {
public:
    /** AP_Drive Constructor */
    AP_Drive(AudioIO& audio,int32_t mix = 0, int16_t level = 0, int16_t drive = 0);

    /** Process and apply the settings on the audio */
    void processAudio();

    /** Set the Drive*/
    void setDrive(int16_t drive);

private:
    AudioIO& m_audio;
    float m_drive;

    int16_t applyDrive(int16_t sample);
};

#endif // AP_DRIVE_H
