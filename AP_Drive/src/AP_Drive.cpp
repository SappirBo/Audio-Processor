#include "AP_Drive.hpp"
#include <algorithm>
#include <cmath>

AP_Drive::AP_Drive(AudioIO& audio, int16_t volume, int16_t gain, int16_t drive):
m_audio(audio)
{
    setVolume((volume > 10) ? 10 : (volume < -10) ? -10 : volume);
    setGain((gain > 10) ? 10 : (gain < -10) ? -10 : gain);
    setDrive((drive > 10) ? 10 : (drive < -10) ? -10 : drive);
}

void AP_Drive::setVolume(int16_t v) {
    m_volume = v;
}

void AP_Drive::setGain(int16_t g) {
    m_gain = g;
}

void AP_Drive::setDrive(int16_t d) {
    m_drive = d;
}

void AP_Drive::processAudio() {    
    
    for (auto& sample : m_audio.getSamples()) {
        sample = applyGain(sample);
        sample += applyDrive(sample);
    }
}

int16_t AP_Drive::applyGain(int16_t sample) {
    double gain_level_scaled{static_cast<double>(m_gain)/100};

    double gain_factor{gain_level_scaled * static_cast<double>(sample)};

    int64_t temp_sample = static_cast<int64_t>(sample) + static_cast<int64_t>(gain_factor);
    
    // Clipping to int32_t limits to prevent overflow
    if      (temp_sample > INT32_MAX) temp_sample = INT32_MAX;
    else if (temp_sample < INT32_MIN) temp_sample = INT32_MIN;
    
    return static_cast<int32_t>(temp_sample);
}

int16_t AP_Drive::applyDrive(int16_t sample) {
    float drive_to_apply{float(sample * m_drive) / INT16_MAX};
    float theta = std::atan(drive_to_apply);
    int16_t drive = int16_t(INT16_MAX*theta) ;
    return abs(drive) < INT16_MAX? drive : INT16_MAX ;
}
