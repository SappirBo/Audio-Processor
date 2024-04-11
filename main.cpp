#include "AP_AudioIO/include/AudioIO.hpp"
#include <iostream>

int main() {
    AudioIO audioIO("/home/sappirb/code/Audio-Processor/data/data1.wav");

    if (!audioIO.load()) {
        std::cerr << "Failed to load audio." << std::endl;
        return 1;
    }

    std::cout << audioIO ;

  
    float gain = 1;  
    for (auto& sample : audioIO.getSamples()) {
        sample = static_cast<int32_t>(sample * gain);
    }

    if (!audioIO.save("/home/sappirb/code/Audio-Processor/data/output.wav")) {
        std::cerr << "Failed to save audio." << std::endl;
        return 1;
    }

    std::cout << "Audio processing completed successfully." << std::endl;
    return 0;
}
