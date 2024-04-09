#include "AP_AudioIO/include/AudioIO.hpp"
#include <iostream>

int main() {
    AudioIO audioIO("data/data1.wav");
    if (!audioIO.load()) {
        std::cerr << "Failed to load audio." << std::endl;
        return 1;
    }

    // Process samples (e.g., apply gain)
    float gain = 1.5;  // Example gain
    for (auto& sample : audioIO.getSamples()) {
        sample = static_cast<short>(sample * gain);
    }

    if (!audioIO.save("output.wav")) {
        std::cerr << "Failed to save audio." << std::endl;
        return 1;
    }

    std::cout << "Audio processing completed successfully." << std::endl;
    return 0;
}
