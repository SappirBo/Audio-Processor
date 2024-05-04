#ifndef AP_DRIVE_H
#define AP_DRIVE_H

#include "../../AP_AudioIO\include/AP_AudioIO.hpp"
#include "../../AP_Audio_Effect/include/AP_Audio_Effect.hpp"
#include <iostream>

class AP_Drive: public AP_Audio_Effect {
public:
    /** AP_Drive Constructor */
    AP_Drive(AP_AudioIO& audio,int32_t mix = 0, int16_t level = 0, int16_t drive = 0);

    /** Process and apply the settings on the audio */
    void processAudio();

    /** Set the Drive*/
    void setDrive(int16_t drive);

private:
    // AP_AudioIO& m_audio;
    float m_drive;

    int16_t applyDrive(int16_t sample);
};

#endif // AP_DRIVE_H
