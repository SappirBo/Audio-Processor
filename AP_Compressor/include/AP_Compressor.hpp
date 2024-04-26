#ifndef AP_COMPRESSOR_HPP
#define AP_COMPRESSOR_HPP

#include <AP_Audio_Effect.hpp>

class AP_Compressor: public AP_Audio_Effect
{
private:
    float m_threshhold;
    float m_ratio;
    float m_attack;
    float m_release;
public:
    AP_Compressor(AP_AudioIO& _audio, int32_t _mix = 0, int32_t _level = 0, 
        float _threshhold = 0, float _ratio = 1.0, float _attack = 1.0, float _release = 100.0);
    ~AP_Compressor();

    void processAudio();
};

// AP_Compressor::AP_Compressor(/* args */)
// {
// }

// AP_Compressor::~AP_Compressor()
// {
// }



#endif