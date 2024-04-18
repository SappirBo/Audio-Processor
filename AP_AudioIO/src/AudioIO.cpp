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

    m_samples.clear();
    int16_t buffer;

    while (file.read(reinterpret_cast<char*>(&buffer), sizeof(buffer))) {
        m_samples.push_back(buffer);
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

    int preAudioPosition = file.tellp();
    for (auto sample : m_samples) {
        file.write(reinterpret_cast<const char*>(&sample), sizeof(sample));
    }
    int postAudioPosition = file.tellp();

    file.seekp(4, std::ios::beg);
    writeToFile(file, postAudioPosition - 8, 4);


    file.close();
    return true;
}

std::vector<int16_t>& AudioIO::getSamples() {
    return m_samples;
}

std::string AudioIO::getFilePath() const
{
    return filePath;
}

bool AudioIO::readHeader(std::ifstream& file) {
        char buffer[44]; // Buffer large enough for the standard header.
        if (!file.read(buffer, 44)) {
            return false;
        }

        // Extracting the RIFF header and checking file type
        m_RIFF = std::string(buffer, 4);
        if (m_RIFF != "RIFF") {
            std::cerr << "Not a valid RIFF file." << std::endl;
            return false;
        }

        m_FileSize = *reinterpret_cast<int32_t*>(buffer + 4);
        m_WAVE = std::string(buffer + 8, 4);
        if (m_WAVE != "WAVE") {
            std::cerr << "Not a valid WAVE file." << std::endl;
            return false;
        }

        // "fmt " subchunk
        m_fmt = std::string(buffer + 12, 4);
        m_subchunkSize = *reinterpret_cast<int32_t*>(buffer + 16);
        // Check if the next chunk is "fmt " or a "JUNK" chunk
        if (m_fmt == "JUNK") {
            std::cout << "Location = " << file.tellg() << "\n";
            return readHeaderwithJunk(file, buffer);
        }
        else{
            return readHeaderwithoutJunk(file, buffer);
        }
        
        return true;
    }

bool AudioIO::readHeaderwithJunk(std::ifstream& file, char buffer[])
{
    // Skip the JUNK chunk
    int32_t junk_size = m_subchunkSize;
    int32_t junk_start_point = 20;
    file.seekg(junk_start_point + junk_size, std::ios_base::beg);  // Move file pointer after the JUNK data

    // Read the actual fmt chunk after the JUNK
    file.read(buffer, 32); // Read the next 24 bytes which should include "fmt " and its content
    m_fmt = std::string(buffer, 4);
    m_subchunkSize = *reinterpret_cast<int32_t*>(buffer + 4);
    // Read remaining fmt data if fmt is correct
    if (m_fmt != "fmt ") 
    {
        std::cerr << "Format chunk missing or not recognized." << std::endl;
        return false;
    }

    m_audioFormat = *reinterpret_cast<int16_t*>(buffer + 8);
    m_numChannels = *reinterpret_cast<int16_t*>(buffer + 10);
    m_sampleRate = *reinterpret_cast<int32_t*>(buffer + 12);
    m_byteRate = *reinterpret_cast<int32_t*>(buffer + 16);
    m_blockAlign = *reinterpret_cast<int16_t*>(buffer + 20);
    m_bitsPerSample = *reinterpret_cast<int16_t*>(buffer + 22);

    m_data = std::string(buffer + 24, 4);
    m_dataSize = *reinterpret_cast<int32_t*>(buffer + 28); // Number of bytes in the data.
    return true;
}

bool AudioIO::readHeaderwithoutJunk(std::ifstream& file, char buffer[])
{
    // Read remaining fmt data if fmt is correct
    if (m_fmt != "fmt ") {
        std::cerr << "Format chunk missing or not recognized." << std::endl;
        return false;
    }
    
    // file.read(buffer + 24, m_subchunkSize);  // Read the rest of the fmt chunk
    m_audioFormat = *reinterpret_cast<int16_t*>(buffer + 20);
    m_numChannels = *reinterpret_cast<int16_t*>(buffer + 22);
    m_sampleRate = *reinterpret_cast<int32_t*>(buffer + 24);
    m_byteRate = *reinterpret_cast<int32_t*>(buffer + 28);
    m_blockAlign = *reinterpret_cast<int16_t*>(buffer + 32);
    m_bitsPerSample = *reinterpret_cast<int16_t*>(buffer + 34);

    m_data = std::string(buffer + 36, 4);
    m_dataSize = *reinterpret_cast<int32_t*>(buffer + 40); // Number of bytes in the data.

    return true;
}

bool AudioIO::writeHeader(std::ofstream& audio_file){
    //Header Chunck
    audio_file << "RIFF";
    audio_file << "----";
    audio_file << "WAVE";

    //Format Chunck
    audio_file << m_fmt.c_str();
    writeToFile(audio_file,m_subchunkSize,4);  // Size
    writeToFile(audio_file,m_audioFormat,2);   // Compression Code
    writeToFile(audio_file,m_numChannels,2);   // Number Of Chanels
    writeToFile(audio_file,m_sampleRate,4); //Sample Rate
    int32_t byteRate = m_sampleRate * m_numChannels * (m_bitsPerSample / 8);
    writeToFile(audio_file, byteRate, 4 ); // byte rate
    uint16_t blockAlign = m_numChannels * (m_bitsPerSample / 8);
    writeToFile(audio_file,blockAlign,2); // block align
    
    writeToFile(audio_file,m_bitsPerSample,2); // bit depth

    //Data Chunck
    audio_file << "data";
    audio_file << "----";

    return true;
}

void AudioIO::writeToFile(std::ofstream &file, int32_t value, int32_t size) {
    file.write(reinterpret_cast<const char*> (&value), size);
}

std::ostream& operator<< (std::ostream& out, const AudioIO& obj)
{
    out << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n";
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


void AudioIO::analyzeWAV() {
    std::ifstream file(filePath, std::ios::binary);
    WAVHeader header;

    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return;
    }

    // Reading the header
    file.read(reinterpret_cast<char*>(&header), sizeof(WAVHeader));

    // Check if the headers are correct
    if (strncmp(header.chunkID, "RIFF", 4) != 0 || strncmp(header.format, "WAVE", 4) != 0) {
        std::cerr << "This is not a valid WAV file." << std::endl;
        return;
    }

    // Output the information
    std::cout << "Number of Channels: " << header.numChannels << std::endl;
    std::cout << "Sample Rate: " << header.sampleRate << std::endl;
    std::cout << "Bits Per Sample: " << header.bitsPerSample << std::endl;
    std::cout << "Data Size: " << header.subchunk2Size << " bytes" << std::endl;

    file.close();
}


void AudioIO::setSamples(std::vector<int16_t> samples)
{
    m_samples = samples;
}