#include "AP_Drive.hpp"
#include <algorithm>
#include <cmath>

AP_Drive::AP_Drive(AudioIO& audio, float volume, float gain, float drive)
    : audio(audio), volume(volume), gain(gain), drive(drive) {}

void AP_Drive::setVolume(float v) {
    volume = v;
}

void AP_Drive::setGain(float g) {
    gain = g;
}

void AP_Drive::setDrive(float d) {
    drive = d;
}

void AP_Drive::processAudio() {
    std::vector<int32_t>& samples = audio.getSamples();
    for (auto& sample : samples) {
        float processedSample = static_cast<float>(sample);
        
        processedSample = applyGain(processedSample, gain);

        processedSample = applyDrive(processedSample, drive);

        processedSample *= volume;

        // Clip to prevent overflow
        processedSample = std::max(std::min(processedSample, static_cast<float>(INT32_MAX)), static_cast<float>(INT32_MIN));

        sample = static_cast<int32_t>(processedSample);
    }
}

float AP_Drive::applyGain(float sample, float gainLevel) {
    return sample * gainLevel;
}

float AP_Drive::applyDrive(float sample, float driveLevel) {
    // Soft clipping formula: a form of distortion
    float theta = std::atan(sample * driveLevel);
    return theta;
}
