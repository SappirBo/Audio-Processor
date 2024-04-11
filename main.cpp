#include "AP_AudioIO/include/AudioIO.hpp"
#include <iostream>

int main() {
    AudioIO audioIO("/home/sappirb/code/Audio-Processor/data/AmrDiabTamallyMaak.wav");

    if (!audioIO.load()) {
        std::cerr << "Failed to load audio." << std::endl;
        return 1;
    }

    std::cout << audioIO ;

    // Process samples (e.g., apply gain)
    // float gain = 1;  // Example gain
    // for (auto& sample : audioIO.getSamples()) {
    //     sample = static_cast<short>(sample * gain);
    // }

    if (!audioIO.save("/home/sappirb/code/Audio-Processor/data/output.wav")) {
        std::cerr << "Failed to save audio." << std::endl;
        return 1;
    }

    std::cout << "Audio processing completed successfully." << std::endl;
    return 0;
}
