#ifndef AP_WAVPLAYER_HPP
#define AP_WAVPLAYER_HPP

#include "AP_AudioIO.hpp"
#include <portaudio.h>
#include <iostream>

class AP_WavPlayer
{
private:
    AP_AudioIO& m_audioIO;
    PaStream* m_stream;

    bool openStream();
    void closeStream();
    
public:
    AP_WavPlayer(AP_AudioIO& audio);
    ~AP_WavPlayer();
    bool initializeAudioStream();
    bool startPlayback();
    bool stopPlayback();
    static int32_t audioCallback(void* outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo* timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void* userData);
};

#endif