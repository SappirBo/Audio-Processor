#ifndef AP_DELAY_H
#define AP_DELAY_H

#include "AudioIO.hpp"
#include <iostream>

class AP_Delay
{
private:
    AudioIO& m_audio;
    float m_feedback;
    float m_delay; // ms
    float m_mix; // 0-1
    std::vector<int16_t> m_delay_buffer;
    size_t write_index{0};

public:
    AP_Delay(AudioIO& audio, float feedback ,float delay, float mix);
    void setFeedBack(float f);
    void setDelay(float t);
    void setMix(float m);
    ~AP_Delay();

    /** Process and apply the settings on the audio */
    void processAudio();

private:
    int16_t applyDelay(int16_t sample);

};

#endif