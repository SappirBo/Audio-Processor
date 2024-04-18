#ifndef AUDIOIO_H
#define AUDIOIO_H

#include <vector>
#include <string>
#include <fstream>

struct WAVHeader {
    char chunkID[4];
    int32_t chunkSize;
    char format[4];
    char subchunk1ID[4];
    uint32_t subchunk1Size;
    uint16_t audioFormat;
    int16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char subchunk2ID[4];
    uint32_t subchunk2Size;
};


class AudioIO {
public:
    AudioIO(const std::string& filename);
    bool load();
    bool save(const std::string& filename);
    std::vector<int16_t>& getSamples();
    std::string getFilePath() const;
    const std::string& getRIFF() const { return m_RIFF; }
    int32_t getFileSize() const { return m_FileSize; }
    const std::string& getWAVE() const { return m_WAVE; }
    const std::string& getFmt() const { return m_fmt; }
    int32_t getSubchunkSize() const { return m_subchunkSize; }
    int16_t getAudioFormat() const { return m_audioFormat; }
    int16_t getNumChannels() const { return m_numChannels; }
    int32_t getSampleRate() const { return m_sampleRate; }
    int32_t getByteRate() const { return m_byteRate; }
    int16_t getBlockAlign() const { return m_blockAlign; }
    int16_t getBitsPerSample() const { return m_bitsPerSample; }
    const std::string& getData() const { return m_data; }
    int32_t getDataSize() const { return m_dataSize; }

    void analyzeWAV();
    

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
    int32_t m_sampleRate;
    int32_t m_byteRate;
    uint16_t m_blockAlign;
    uint16_t m_bitsPerSample;

    std::string m_data;
    int32_t m_dataSize;

    std::vector<int16_t> m_samples;

    bool readHeader(std::ifstream& file);
    bool writeHeader(std::ofstream& file);

    bool readHeaderwithJunk(std::ifstream& file, char buffer[]);
    bool readHeaderwithoutJunk(std::ifstream& file, char buffer[]);

    void writeToFile(std::ofstream &file, int32_t value, int32_t size);
};

#endif // AUDIOIO_H
