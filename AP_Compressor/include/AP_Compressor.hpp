#ifndef AP_COMPRESSOR_HPP
#define AP_COMPRESSOR_HPP

#include <AP_Audio_Effect.hpp>

enum class compressionSpeed
{
    

};

class AP_Compressor: public AP_Audio_Effect
{
private:
    /* m_threashold is set to be number from 0 to -100 ([0,-100]) as 0 in max amplitude*/
    float m_threshold;

    /** m_ratio is set from 1 (that means no change) to 8 (max compress)*/ 
    float m_ratio;
    float m_attack;  //ms
    float m_release; //ms
public:
    AP_Compressor(AP_AudioIO& _audio, int32_t _mix = 0, int32_t _level = 0, 
        float _threshold = 0, float _ratio = 1.0, float _attack = 10.0, float _release = 100.0);
    ~AP_Compressor() = default;

    void processAudio();
    void setThreshold(float _t);
    void setRatio(float _r);
    void setAttack(float _a);
    void setRelease(float _r);

private:
    void applyCompression();
    void compress(int16_t& _sample, float _refinement);
};
#endif