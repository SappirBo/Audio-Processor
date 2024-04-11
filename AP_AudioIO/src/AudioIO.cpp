#include "AudioIO.hpp"
#include <iostream>

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

int AudioIO::getSampleRate() const {
    return sampleRate;
}

int AudioIO::getBitsPerSample() const {
    return bitsPerSample;
}

int AudioIO::getNumChannels() const {
    return numChannels;
}

bool AudioIO::readHeader(std::ifstream& file) {
    char buffer[44];
    if (!file.read(buffer, 44)) return false;

    // Parse header here
    sampleRate = *reinterpret_cast<int*>(buffer + 24);
    bitsPerSample = *reinterpret_cast<int*>(buffer + 34);
    numChannels = *reinterpret_cast<int*>(buffer + 22);
    return true;
}

bool AudioIO::writeHeader(std::ofstream& file) {
    char buffer[44] = {0};

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