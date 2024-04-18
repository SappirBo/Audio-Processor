#include "AP_Drive.hpp"
#include <algorithm>
#include <cmath>

AP_Drive::AP_Drive(AudioIO& audio, int16_t drive):
m_audio(audio)
{
    setDrive((drive > 100) ? 100 : (drive < 0)? 0: drive);
}


void AP_Drive::setDrive(int16_t d) {
    m_drive = float(d)/20;
}

void AP_Drive::processAudio() {    
    for (auto& sample : m_audio.getSamples()) {
        sample += applyDrive(sample);

        sample = (sample > INT16_MAX ? INT16_MAX : (sample < INT16_MIN? INT16_MIN: sample));
    }
}

int16_t AP_Drive::applyDrive(int16_t sample) {
    
    float drive_to_apply{float(sample * m_drive  ) / INT16_MAX};

    float theta = std::atan(drive_to_apply);

    int16_t drive = int16_t(INT16_MAX*theta) ;

    return abs(drive) < INT16_MAX? drive : INT16_MAX ;
}
