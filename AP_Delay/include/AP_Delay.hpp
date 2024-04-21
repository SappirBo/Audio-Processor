#ifndef AP_DELAY_H
#define AP_DELAY_H

#include "AP_AudioIO.hpp"
#include "AP_Audio_Effect.hpp"
#include <iostream>

class AP_Delay: public AP_Audio_Effect
{
private:
    float m_feedback;
    float m_delay; // ms
    std::vector<int16_t> m_delay_buffer;
    size_t write_index{0};

public:
    AP_Delay(AP_AudioIO& audio,float mix, float feedback ,float delay);
    void setFeedBack(float f);
    void setDelay(float t);
    void setMix(float m);
    ~AP_Delay();

    /** Process and apply the settings on the audio */
    void processAudio();

private:
    int16_t applyDelay(int16_t sample);
    std::vector<int16_t> getRemainingSamples();

};

#endif