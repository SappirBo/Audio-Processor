#include "AP_AudioIO/include/AudioIO.hpp"
#include "AP_Drive/include/AP_Drive.hpp"
#include "AP_Delay/include/AP_Delay.hpp"
#include "AP_Audio_Effect/include/AP_Audio_Effect.hpp"
#include <iostream>
#include <vector>

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

    // std::cout << audioIO ; 

    // Drive is set to be 0-100
    AP_Drive drive{audioIO,20, 70, 60};
    drive.processAudio();

    // std::cout << "- - - Before delay: " << audioIO.getSamples().size() ;
    
    // Delay time is best in 3000 to 2 bars
    AP_Delay delay{audioIO,20, 200, 2225};
    delay.processAudio();
    
    // std::cout << "| After delay: " << audioIO.getSamples().size() << std::endl;
    
    
    if (!audioIO.save("/home/sappirb/code/Audio-Processor/output/output.wav")) {
        std::cerr << "Failed to save audio." << std::endl;
        return 1;
    }

    // AudioIO audioIO_out("/home/sappirb/code/Audio-Processor/output/output.wav");
    // std::cout << audioIO_out ; 

    std::cout << "Audio processing completed successfully." << std::endl;
    return 0;
}
