#include "AP_Compressor.hpp"

AP_Compressor::AP_Compressor(AP_AudioIO& _audio, int32_t _mix = 0, int32_t _level = 0, 
        float _threshold = 0, float _ratio = 1.0, float _attack = 1.0, float _release = 100.0):
        AP_Audio_Effect(_audio,_mix,_level)
{
    setThreshold(_threshold);
    setRatio(_ratio);
    setAttack(_attack);
    setRatio(_release);
}

void AP_Compressor::setThreshold(float _t)
{
    if(_t >= 0.0){m_threshold = 0.0;}
    else{m_threshold = _t;}
}

void AP_Compressor::setRatio(float _r)
{
    if(_r <= 1.0){m_ratio = 1.0;}
    else{ m_ratio = _r;}
}

void AP_Compressor::setAttack(float _a)
{
    m_attack = _a;
}

void AP_Compressor::setRelease(float _r)
{
    m_release = _r;
}