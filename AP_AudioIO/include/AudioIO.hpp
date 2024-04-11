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
    std::vector<short>& getSamples();
    std::string getFilePath() const;
    uint32_t getSampleRate() const;
    short getBitsPerSample() const;
    short getNumChannels() const;
    

    friend std::ostream& operator<< (std::ostream& out, const AudioIO& obj); 

private:
    std::string filePath;
    uint32_t sampleRate;
    uint32_t dataSize;
    short bitsPerSample;
    short numChannels;
    std::vector<short> samples;

    bool readHeader(std::ifstream& file);
    bool writeHeader(std::ofstream& file);
};

#endif // AUDIOIO_H
