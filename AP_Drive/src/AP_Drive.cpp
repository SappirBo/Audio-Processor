#include "AP_Drive.hpp"
#include <algorithm>
#include <cmath>

AP_Drive::AP_Drive(AudioIO& audio, int32_t volume, int32_t gain, int32_t drive):
m_audio(audio)
{
    setVolume((volume > 10) ? 10 : (volume < -10) ? -10 : volume);
    setGain((gain > 10) ? 10 : (gain < -10) ? -10 : gain);
    setDrive((drive > 10) ? 10 : (drive < -10) ? -10 : drive);
}

void AP_Drive::setVolume(int32_t v) {
    m_volume = v;
}

void AP_Drive::setGain(int32_t g) {
    m_gain = g;
}

void AP_Drive::setDrive(int32_t d) {
    m_drive = d;
}

void AP_Drive::processAudio() {
    std::vector<int32_t>& samples = m_audio.getSamples();
    float scaleFactor = (m_audio.getBitsPerSample() == 16) ? 32768.0f : 2147483648.0f;

    for (auto& sample : samples) {
        float normalizedSample = sample / scaleFactor;
        normalizedSample = applyGain(normalizedSample);
        normalizedSample = applyDrive(normalizedSample);
        normalizedSample *= m_volume;
        normalizedSample = std::max(std::min(normalizedSample, 1.0f), -1.0f);
        sample = static_cast<int32_t>(normalizedSample * scaleFactor);
    }
}

int32_t AP_Drive::applyGain(int32_t sample) {
    double gain_level_scaled{static_cast<double>(m_gain)/100};
    double gain_factor{gain_level_scaled * static_cast<double>(sample)};
    int64_t temp_sample = static_cast<int64_t>(sample) + static_cast<int64_t>(gain_factor);
    
    // Clipping to int32_t limits to prevent overflow
    if (temp_sample > INT32_MAX) temp_sample = INT32_MAX;
    else if (temp_sample < INT32_MIN) temp_sample = INT32_MIN;
    
    return static_cast<int32_t>(temp_sample);
}

int32_t AP_Drive::applyDrive(int32_t sample) {
    int32_t drive_to_apply{sample * m_drive};
    int32_t theta = std::atan(drive_to_apply);
    return theta;
}
