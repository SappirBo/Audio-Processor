#include "AudioIO.hpp"
#include <iostream>
#include <cstring> 

AudioIO::AudioIO(const std::string& filename) : filePath(filename) {}

bool AudioIO::load() {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return false;
    }

    if (!readHeader(file)) {
        std::cerr << "Failed to read header from file: " << filePath << std::endl;
        return false;
    }

    samples.clear();
    short buffer;
    while (file.read(reinterpret_cast<char*>(&buffer), sizeof(buffer))) 
    {
        samples.push_back(buffer);
    }

    file.close();
    return true;
}

bool AudioIO::save(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return false;
    }

    if (!writeHeader(file)) {
        std::cerr << "Failed to write header to file: " << filename << std::endl;
        return false;
    }

    for (auto sample : samples) {
        file.write(reinterpret_cast<const char*>(&sample), sizeof(sample));
    }

    file.close();
    return true;
}

std::vector<short>& AudioIO::getSamples() {
    return samples;
}

std::string AudioIO::getFilePath() const
{
    return filePath;
}

uint32_t AudioIO::getSampleRate() const {
    return sampleRate;
}

short AudioIO::getBitsPerSample() const {
    return bitsPerSample;
}

short AudioIO::getNumChannels() const {
    return numChannels;
}

bool AudioIO::readHeader(std::ifstream& file) {
    char buffer[44];
    if (!file.read(buffer, 44))
    {
        return false;
    };

    sampleRate    = *reinterpret_cast<int*>(buffer + 24);
    bitsPerSample = *reinterpret_cast<short*>(buffer + 34);
    numChannels   = *reinterpret_cast<short*>(buffer + 22);
    dataSize      = sampleRate * numChannels * (bitsPerSample / 8);
    return true;
}

bool AudioIO::writeHeader(std::ofstream& file) {
    char buffer[44] = {0};

    // RIFF header
    std::memcpy(buffer, "RIFF", 4);
    uint32_t fileSize = 36 + dataSize; 
    std::memcpy(buffer + 4, &fileSize, 4);

    // WAVE header
    std::memcpy(buffer + 8, "WAVE", 4);

    // fmt subchunk
    std::memcpy(buffer + 12, "fmt ", 4);
    uint32_t subchunk1Size = 16; // PCM header size
    std::memcpy(buffer + 16, &subchunk1Size, 4);
    uint16_t audioFormat = 1; // PCM
    std::memcpy(buffer + 20, &audioFormat, 2);
    std::memcpy(buffer + 22, &numChannels, 2);
    std::memcpy(buffer + 24, &sampleRate, 4);
    uint32_t byteRate = sampleRate * numChannels * bitsPerSample / 8;
    std::memcpy(buffer + 28, &byteRate, 4);
    uint16_t blockAlign = numChannels * bitsPerSample / 8;
    std::memcpy(buffer + 32, &blockAlign, 2);
    std::memcpy(buffer + 34, &bitsPerSample, 2);

    // data subchunk
    std::memcpy(buffer + 36, "data", 4);
    std::memcpy(buffer + 40, &dataSize, 4); // dataSize: number of bytes in audio data

    file.write(buffer, 44);
    return file.good();
}

std::ostream& operator<< (std::ostream& out, const AudioIO& obj)
{
    out << "File Path: "<< obj.getFilePath() << "\n";
    out << "Sample Rate: "<< obj.getSampleRate() << "\n";
    out << "Bits Per Sample: "<< obj.getBitsPerSample() << "\n";
    out << "Number Of Channels: "<< obj.getNumChannels() << "\n";

    return out;
}