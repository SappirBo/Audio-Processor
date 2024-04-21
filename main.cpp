#include "AP_AudioIO/include/AudioIO.hpp"
#include "AP_Drive/include/AP_Drive.hpp"
#include "AP_Delay/include/AP_Delay.hpp"
#include "AP_Audio_Effect/include/AP_Audio_Effect.hpp"
#include <iostream>
#include <vector>
#include <memory>


std::vector<std::string> data_path{
    "/home/sappirb/code/Audio-Processor/data/Audio_Processor_StereoOut.wav",
    "/home/sappirb/code/Audio-Processor/data/Audio_Processor_Bass.wav",
    "/home/sappirb/code/Audio-Processor/data/Audio_Processor_Guitar.wav",
    "/home/sappirb/code/Audio-Processor/data/Audio_Processor_Drums.wav",
    "/home/sappirb/code/Audio-Processor/data/Audio_Processor_Piano.wav",
    "/home/sappirb/code/Audio-Processor/data/data1.wav",
    "/home/sappirb/code/Audio-Processor/output/output.wav",
    "/home/sappirb/code/Audio-Processor/data/80_BPM_Stereo Out.wav"
};

int main() {
    AudioIO audioIO(data_path.at(4));

    if (!audioIO.load()) {
        std::cerr << "Failed to load audio." << std::endl;
        return 1;
    }

    std::vector<std::unique_ptr<AP_Audio_Effect>> effect_chain;

    // Drive is set to be 0-100
    effect_chain.push_back(std::make_unique<AP_Drive>(audioIO,50, 95, 100));
    // Delay time is best in 3000 to 2 bars
    effect_chain.push_back(std::make_unique<AP_Delay>(audioIO,35, 200, 2225));

    for(auto& effect : effect_chain)
    {
        effect->processAudio();
    }
    
    if (!audioIO.save("/home/sappirb/code/Audio-Processor/output/output.wav")) {
        std::cerr << "Failed to save audio." << std::endl;
        return 1;
    }

    std::cout << "Audio processing completed successfully." << std::endl;
    return 0;
}
