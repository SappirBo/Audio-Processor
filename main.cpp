#include "AP_AudioIO/include/AP_AudioIO.hpp"
#include "AP_Drive/include/AP_Drive.hpp"
#include "AP_Delay/include/AP_Delay.hpp"
#include "AP_Compressor/include/AP_Compressor.hpp"
#include "AP_Audio_Effect/include/AP_Audio_Effect.hpp"
#include "AP_WavPlayer/include/AP_WavPlayer.hpp"
#include <iostream>
#include <vector>
#include <memory>


std::vector<std::string> data_path{
    "/home/sappirb/code/Audio-Processor/data/Audio_Processor_StereoOut.wav",
    "/home/sappirb/code/Audio-Processor/data/Audio_Processor_Bass.wav",
    "/home/sappirb/code/Audio-Processor/data/Audio_Processor_Guitar.wav",
    "/home/sappirb/code/Audio-Processor/data/Audio_Processor_Drums.wav",
    "/home/sappirb/code/Audio-Processor/data/Audio_Processor_Piano.wav",
    "/home/sappirb/code/Audio-Processor/data/Audio_Processor_Clap.wav",
    "/home/sappirb/code/Audio-Processor/data/Audio_Processor_SlowPiano.wav",
    "/home/sappirb/code/Audio-Processor/data/Audio_Processor_Snap.wav",
    "/home/sappirb/code/Audio-Processor/data/Audio_Processor_Snare.wav",
    "/home/sappirb/code/Audio-Processor/data/data1.wav",
    "/home/sappirb/code/Audio-Processor/output/output.wav",
    "/home/sappirb/code/Audio-Processor/data/80_BPM_Stereo Out.wav"
};

int main() {
    AP_AudioIO audio(data_path.at(6));

    if (!audio.load()) {
        std::cerr << "Failed to load audio." << std::endl;
        return 1;
    }

    std::cout << audio << std::endl;

    // std::vector<std::unique_ptr<AP_Audio_Effect>> effect_chain;

    // // Drive is set to be 0-100
    // effect_chain.push_back(std::make_unique<AP_Drive>(AP_AudioIO,70, 50, 60));
    
    // // Delay time is best in 3000 to 2 bars
    // effect_chain.push_back(std::make_unique<AP_Delay>(AP_AudioIO,30, 600, 2225));

    // for(auto& effect : effect_chain)
    // {
    //     effect->processAudio();
    // }
    
    if (!audio.save("/home/sappirb/code/Audio-Processor/output/output.wav")) {
        std::cerr << "Failed to save audio." << std::endl;
        return 1;
    }

    std::cout << "Audio processing completed successfully." << std::endl;
    return 0;
}
