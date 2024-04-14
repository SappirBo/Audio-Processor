#include "AP_AudioIO/include/AudioIO.hpp"
#include "AP_Drive/include/AP_Drive.hpp"
#include <iostream>
#include <vector>

std::vector<std::string> data_path{
    "/home/sappirb/code/Audio-Processor/data/Audio_Processor_StereoOut.wav",
    "/home/sappirb/code/Audio-Processor/data/Audio_Processor_Bass.wav",
    "/home/sappirb/code/Audio-Processor/data/Audio_Processor_Guitar.wav",
    "/home/sappirb/code/Audio-Processor/data/Audio_Processor_Drums.wav",
    "/home/sappirb/code/Audio-Processor/data/Audio_Processor_Piano.wav"
};

int main() {
    AudioIO audioIO(data_path.at(0));

    if (!audioIO.load()) {
        std::cerr << "Failed to load audio." << std::endl;
        return 1;
    }

    std::cout << audioIO ; 

    // AP_Drive drive{audioIO,1,1,1};
    // drive.processAudio();

    if (!audioIO.save("/home/sappirb/code/Audio-Processor/output/output.wav")) {
        std::cerr << "Failed to save audio." << std::endl;
        return 1;
    }

    std::cout << "Audio processing completed successfully." << std::endl;
    return 0;
}
