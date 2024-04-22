#include "AP_WavPlayer.hpp"

AP_WavPlayer::AP_WavPlayer(AP_AudioIO& audio)
:m_audioIO(audio)
{
}

AP_WavPlayer::~AP_WavPlayer()
{
}

int32_t AP_WavPlayer::audioCallback(void* outputBuffer,
                                unsigned long framesPerBuffer,
                                const PaStreamCallbackTimeInfo* timeInfo,
                                PaStreamCallbackFlags statusFlags,
                                void* userData)
{
    AP_AudioIO* audio = static_cast<AP_AudioIO*>(userData);
    auto& samples = audio->getSamples();
    std::size_t totalFrames = samples.size() / audio->getNumChannels();

    // Convert and copy the audio samples to outputBuffer
    int16_t* out = static_cast<int16_t*>(outputBuffer);

    for (unsigned long i = 0; i < framesPerBuffer; ++i)
    {
        if (i >= totalFrames) break;
        for (int channel = 0; channel < audio->getNumChannels(); ++channel)
        {
            *out++ = samples[i * audio->getNumChannels() + channel];
        }
    }
    return paContinue;
}


