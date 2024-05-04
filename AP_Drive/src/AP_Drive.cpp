#include "../include/AP_Drive.hpp"
#include <algorithm>
#include <cmath>

AP_Drive::AP_Drive(AP_AudioIO& audio,int32_t mix, int16_t level, int16_t drive):
AP_Audio_Effect(audio, mix, level)
{
    setDrive((drive > 100) ? 100 : (drive < 0)? 0: drive);
}

void AP_Drive::setDrive(int16_t d) {
    m_drive = float(d)/100;
}

void AP_Drive::processAudio() {    
    for (auto& sample : m_audio.getSamples()) 
    {
        int16_t drived_sample = applyDrive(sample);
        float scaled_with_mix = (m_mix) * float(drived_sample) + (1-m_mix) * float(sample); 
        sample = int16_t(scaled_with_mix); 
        sample = applyLevel(sample);
    }
}

int16_t AP_Drive::applyDrive(int16_t sample) {
    
    float sample_scaled{float(sample) / INT16_MAX};
    float amplitude{10};


    float theta = amplitude * std::atan(sample_scaled * m_drive);

    theta = (theta>0.8? 0.8: (theta<-0.8? -0.8: theta));

    int16_t drive = INT16_MAX * theta;

    return drive ;
}
