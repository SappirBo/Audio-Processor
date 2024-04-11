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
    int32_t buffer;

    while (file.read(reinterpret_cast<char*>(&buffer), sizeof(buffer))) {
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

std::vector<int32_t>& AudioIO::getSamples() {
    return samples;
}

std::string AudioIO::getFilePath() const
{
    return filePath;
}

bool AudioIO::readHeader(std::ifstream& file) {
    char buffer[44]; // This size is based on the standard WAV header size.
    if (!file.read(buffer, 44)) {
        return false;
    }

    // Extracting the RIFF header and checking file type
    m_RIFF = std::string(buffer, 4);
    if (m_RIFF != "RIFF") {
        std::cerr << "Not a valid RIFF file." << std::endl;
        return false;
    }

    m_FileSize = *reinterpret_cast<int32_t*>(buffer + 4); // Total file size minus 8 bytes for 'RIFF' and size
    m_WAVE = std::string(buffer + 8, 4);
    if (m_WAVE != "WAVE") {
        std::cerr << "Not a valid WAVE file." << std::endl;
        return false;
    }

    // "fmt " subchunk
    m_fmt = std::string(buffer + 12, 4);
    m_subchunkSize = *reinterpret_cast<int32_t*>(buffer + 16); // Length of format data as listed above
    m_audioFormat = *reinterpret_cast<int16_t*>(buffer + 20); // Audio format 1 means PCM, which is the standard value
    m_numChannels = *reinterpret_cast<int16_t*>(buffer + 22); // Number of channels
    m_sampleRate = *reinterpret_cast<uint32_t*>(buffer + 24); // Sample rate
    m_byteRate = *reinterpret_cast<uint32_t*>(buffer + 28); // Byte rate
    blockAlign = *reinterpret_cast<int16_t*>(buffer + 32); // Block align
    m_bitsPerSample = *reinterpret_cast<int16_t*>(buffer + 34); // Bits per sample

    // "data" subchunk
    m_data = std::string(buffer + 36, 4);
    m_dataSize = *reinterpret_cast<uint32_t*>(buffer + 40); // Number of bytes in the data.

    return true;
}

bool AudioIO::writeHeader(std::ofstream& file) {
    char buffer[44] = {0};

    // RIFF header
    std::memcpy(buffer, m_RIFF.c_str(), 4);
    uint32_t fileSize = 36 + m_dataSize;  
    std::memcpy(buffer + 4, &fileSize, 4);

    // WAVE header
    std::memcpy(buffer + 8, m_WAVE.c_str(), 4);

    // fmt subchunk
    std::memcpy(buffer + 12, m_fmt.c_str(), 4); 
    std::memcpy(buffer + 16, &m_subchunkSize, 4);

    std::memcpy(buffer + 20, &m_audioFormat, 2); 
    std::memcpy(buffer + 22, &m_numChannels, 2); 
    std::memcpy(buffer + 24, &m_sampleRate, 4);

    uint32_t byteRate = m_sampleRate * m_numChannels * (m_bitsPerSample / 8);
    std::memcpy(buffer + 28, &byteRate, 4);

    uint16_t blockAlign = m_numChannels * (m_bitsPerSample / 8);
    std::memcpy(buffer + 32, &blockAlign, 2);

    std::memcpy(buffer + 34, &m_bitsPerSample, 2);

    // data subchunk
    std::memcpy(buffer + 36, m_data.c_str(), 4);
    std::memcpy(buffer + 40, &m_dataSize, 4); 

    file.write(buffer, 44);
    return file.good();
}

std::ostream& operator<< (std::ostream& out, const AudioIO& obj)
{
    out << "File Path: "<< obj.getFilePath() << "\n";
    out << "RIFF: " << obj.getRIFF() <<"\nFile Size: " << obj.getFileSize() << "\n";
    out << "Wave: " << obj.getWAVE() << "\nFMT: " << obj.getFmt() << "\n";
    out << "Sub-Chunk Size: " << obj.getSubchunkSize();
    out << "\nAudio Format: " << obj.getAudioFormat() << "\nNumber Of Channels: " << obj.getNumChannels() << "\n";
    out << "Sample Rate: " << obj.getSampleRate() << "\nByte Rate: " << obj.getByteRate() << "\n";
    out << "Block Align: " << obj.getBlockAlign() << "\nBits Per Sample: " << obj.getBitsPerSample() << "\n";
    out << "Data: " << obj.getData() << "\nData Size: " << obj.getDataSize() << "\n";

    return out;
}
