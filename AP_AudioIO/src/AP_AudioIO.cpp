#include "../include/AP_AudioIO.hpp"
#include <iostream>
#include <cstring> 

AP_AudioIO::AP_AudioIO(const std::string& filename) : filePath(filename) {}

bool AP_AudioIO::load() {
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
    m_samples.erase(m_samples.end() - 1045,m_samples.end() );

    file.close();
    return true;
}

bool AP_AudioIO::save(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return false;
    }

    if (!writeHeader(file)) {
        std::cerr << "Failed to write header to file: " << filename << std::endl;
        return false;
    }

    for (auto sample : m_samples) {
        file.write(reinterpret_cast<const char*>(&sample), sizeof(sample));
    }
    int postAudioPosition = file.tellp();

    file.seekp(4, std::ios::beg);
    writeToFile(file, postAudioPosition - 8, 4);


    file.close();
    return true;
}

std::vector<int16_t>& AP_AudioIO::getSamples() {
    return m_samples;
}

std::string AP_AudioIO::getFilePath() const
{
    return filePath;
}

bool AP_AudioIO::readHeader(std::ifstream& file) {
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

        int32_t current_pos{20};
        int32_t start_point{20};

        // Check if the next chunk is fmt or a un used other chunk
        if(!removeUnusedChuncks(file, buffer, current_pos,start_point))
        {
            std::cout << "Error: got unknown chuncks - could not safely remove them (readHeader)." << std::endl;
            return false;
        }

        return readHeaderData(buffer, start_point);
    }

bool AP_AudioIO::readHeaderData(char buffer[], int32_t start_point)
{
    // Read remaining fmt data if fmt is correct
    if (m_fmt != "fmt ") {
        std::cerr << "Format chunk missing or not recognized (fmt = '"<< m_fmt << "')." << std::endl;
        return false;
    }

    m_audioFormat   = *reinterpret_cast<int16_t*>(buffer + start_point + 0);
    m_numChannels   = *reinterpret_cast<int16_t*>(buffer + start_point + 2);
    m_sampleRate    = *reinterpret_cast<int32_t*>(buffer + start_point + 4);
    m_byteRate      = *reinterpret_cast<int32_t*>(buffer + start_point + 8);
    m_blockAlign    = *reinterpret_cast<int16_t*>(buffer + start_point + 12);
    m_bitsPerSample = *reinterpret_cast<int16_t*>(buffer + start_point + 14);

    m_data = std::string(buffer + start_point + 16, 4);
    m_dataSize = *reinterpret_cast<int32_t*>(buffer + start_point + 20); // Number of bytes in the data.

    return true;
}

bool AP_AudioIO::writeHeader(std::ofstream& audio_file){
    m_dataSize = m_samples.size() * sizeof(int16_t);
    m_FileSize = 36 + m_dataSize;


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
    // audio_file << "----";
    writeToFile(audio_file,m_dataSize,4); // bit depth

    return true;
}

void AP_AudioIO::writeToFile(std::ofstream &file, int32_t value, int32_t size) {
    file.write(reinterpret_cast<const char*> (&value), size);
}

std::ostream& operator<< (std::ostream& out, const AP_AudioIO& obj)
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

void AP_AudioIO::setSamples(std::vector<int16_t> samples)
{
    m_samples = samples;
}

void AP_AudioIO::skipFile(std::ifstream& _file, int32_t _bytes)
{
    _file.seekg(_bytes, std::ios_base::beg);
}

void AP_AudioIO::skipSubChunk(std::ifstream& _file, int32_t _bytes, char (& _buffer)[44])
{
    skipFile(_file, _bytes);
    _file.read(_buffer, 32); // Read the next 24 bytes which should include "fmt " and its content
    m_fmt = std::string(_buffer, 4);
    m_subchunkSize = *reinterpret_cast<int32_t*>(_buffer + 4);
}

void AP_AudioIO::removeChunkAndUpdate(std::ifstream& _file, char (& _buffer)[44], 
        int32_t& current_pos, int32_t& start_point)
{
    int32_t bytes_for_fmt_and_chunk_size{8};
    int32_t prev_chunk_size{m_subchunkSize};
    skipSubChunk(_file, current_pos + m_subchunkSize, _buffer);
    current_pos = current_pos + prev_chunk_size + bytes_for_fmt_and_chunk_size;
    start_point = bytes_for_fmt_and_chunk_size;
}

bool AP_AudioIO::removeUnusedChuncks(std::ifstream& _file, char (& _buffer)[44], 
        int32_t& current_pos, int32_t& start_point)
{
    while(m_fmt != "fmt ")
    {
        if (m_fmt == "JUNK") {
            removeChunkAndUpdate(_file, _buffer, current_pos,start_point);
        }
        else if(m_fmt == "bext")
        {
            removeChunkAndUpdate(_file, _buffer, current_pos,start_point);
        }

        else if(m_fmt == "Fake")
        {
            removeChunkAndUpdate(_file, _buffer, current_pos,start_point);
        }
        else if(m_fmt == "fmt ")
        {
            return true;
        }
        else
        {
            std::cout << "Error: unknown fmt chunk, m_fmt = '" << m_fmt <<"' (removeUnusedChuncks)." << std::endl;
            return false;
        }
    }

    return true;
}


