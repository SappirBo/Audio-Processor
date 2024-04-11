#ifndef AUDIOIO_H
#define AUDIOIO_H

#include <vector>
#include <string>
#include <fstream>

class AudioIO {
public:
    AudioIO(const std::string& filename);
    bool load();
    bool save(const std::string& filename);
    std::vector<int32_t>& getSamples();
    std::string getFilePath() const;
    const std::string& getRIFF() const { return m_RIFF; }
    int32_t getFileSize() const { return m_FileSize; }
    const std::string& getWAVE() const { return m_WAVE; }
    const std::string& getFmt() const { return m_fmt; }
    int32_t getSubchunkSize() const { return m_subchunkSize; }
    int16_t getAudioFormat() const { return m_audioFormat; }
    int16_t getNumChannels() const { return m_numChannels; }
    uint32_t getSampleRate() const { return m_sampleRate; }
    uint32_t getByteRate() const { return m_byteRate; }
    int16_t getBlockAlign() const { return blockAlign; }
    int16_t getBitsPerSample() const { return m_bitsPerSample; }
    const std::string& getData() const { return m_data; }
    uint32_t getDataSize() const { return m_dataSize; }
    

    friend std::ostream& operator<< (std::ostream& out, const AudioIO& obj); 

private:
    std::string filePath;

    std::string m_RIFF;
    int32_t m_FileSize;
    std::string m_WAVE;
    std::string m_fmt;
    int32_t m_subchunkSize;
    int16_t m_audioFormat;
    int16_t  m_numChannels;
    uint32_t m_sampleRate;
    uint32_t m_byteRate;
    uint16_t blockAlign;
    uint16_t m_bitsPerSample;

    std::string m_data;
    uint32_t m_dataSize;

    std::vector<int32_t> samples;

    bool readHeader(std::ifstream& file);
    bool writeHeader(std::ofstream& file);
};

#endif // AUDIOIO_H
