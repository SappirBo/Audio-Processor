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
    const float scaleFactor = INT32_MAX;

    for (auto& sample : samples) {
        float processedSample = static_cast<float>(sample) / scaleFactor; // Normalize to -1 to 1
        
        processedSample = applyGain(processedSample, gain);
        // processedSample = applyDrive(processedSample, drive);
        processedSample *= volume;
        
        // Clip and scale back to original range
        processedSample = std::max(std::min(processedSample, 1.0f), -1.0f);
        sample = static_cast<int32_t>(processedSample * scaleFactor);
    }
}

float AP_Drive::applyGain(float sample, float gainLevel) {
    return sample * gainLevel;
}

float AP_Drive::applyDrive(float sample, float driveLevel) {
    float drive_to_apply{sample * driveLevel};
    float theta = std::atan(drive_to_apply);
    return theta;
}
