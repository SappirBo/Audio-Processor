#include "AP_Drive.hpp"
#include <algorithm>
#include <cmath>

AP_Drive::AP_Drive(AudioIO& audio,int16_t level, int16_t drive):
m_audio(audio)
{
    setDrive((drive > 100) ? 100 : (drive < 0)? 0: drive);
    setLevel((level > 100) ? 100 : (level < 0)? 0: level);
}

void AP_Drive::setDrive(int16_t d) {
    m_drive = float(d)/100;
}

void AP_Drive::setLevel(int16_t l) {
    m_level = l;
}

void AP_Drive::processAudio() {    
    for (auto& sample : m_audio.getSamples()) 
    {
        sample = applyDrive(sample);
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

int16_t AP_Drive::applyLevel(int16_t sample) {
    
    float sample_scaled{float(sample) / INT16_MAX};

    float level_scaled{(float(m_level)/100)};

    float sample_times_leveling{sample_scaled * level_scaled};

    int16_t sample_leveled = int16_t(float(INT16_MAX) * sample_times_leveling) ;

    return sample_leveled ;
}
