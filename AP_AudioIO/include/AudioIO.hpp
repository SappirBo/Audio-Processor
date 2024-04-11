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
    int getSampleRate() const;
    int getBitsPerSample() const;
    int getNumChannels() const;

    friend std::ostream& operator<< (std::ostream& out, const AudioIO& obj); 

private:
    std::string filePath;
    int sampleRate;
    int bitsPerSample;
    int numChannels;
    std::vector<short> samples;

    bool readHeader(std::ifstream& file);
    bool writeHeader(std::ofstream& file);
};

#endif // AUDIOIO_H
