#include "../include/AP_Compressor.hpp"

AP_Compressor::AP_Compressor(AP_AudioIO& _audio, int32_t _mix, int32_t _level, 
        float _threshold, float _ratio, float _attack, float _release):
        AP_Audio_Effect(_audio,_mix,_level)
{
    setThreshold(_threshold);
    setRatio(_ratio);
    setAttack(_attack);
    setRelease(_release);
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

void AP_Compressor::processAudio()
{
    applyCompression();

    for(auto& sample: m_audio.getSamples())
    {
        sample = applyLevel(sample);
    } 
}

void AP_Compressor::compress(int16_t& _sample, float _refinement)
{
    float decrease = static_cast<float>(_sample)/(m_ratio);
    float factor = (m_ratio * _refinement) > 1.0? (m_ratio * _refinement): 1.0;
    if (m_ratio > 1){
        decrease = static_cast<float>(_sample)/factor;
    }  
    _sample = (1- m_mix)*_sample + (m_mix) * static_cast<int16_t>(decrease);

}

void AP_Compressor::applyCompression()
{
    int32_t sample_rate = m_audio.getSampleRate();
    float one_second{1000.0};
    float attack_in_seconds{static_cast<float>(m_attack)/one_second};

    float release_in_seconds{m_release/one_second};

    int32_t bits_to_attack {static_cast<int32_t>(static_cast<float>(sample_rate) * attack_in_seconds) };
    int32_t bits_to_release{static_cast<int32_t>(static_cast<float>(sample_rate) * release_in_seconds)};

    int32_t threshold_range{100};
    int32_t sample_threashold_chunck{INT16_MAX/threshold_range};

    int16_t sample_threashold{static_cast<int16_t>(INT16_MAX)};
    sample_threashold -= static_cast<int16_t>(m_threshold * static_cast<float>(sample_threashold_chunck));

    bool apply_compression{false};
    int32_t attack_counter{0};
    int32_t release_counter{0};

    float refinement{0};

    for(auto& sample: m_audio.getSamples())
    {
        if(abs(sample) >= abs(sample_threashold))
        {
            if(!apply_compression)
            {
                apply_compression = true;
                attack_counter = 0;
            }
            release_counter = 0;
        }

        if (apply_compression)
        {
            if(attack_counter <= bits_to_attack)
            {
                refinement = (static_cast<float>(attack_counter)/static_cast<float>(bits_to_attack));
                compress(sample, refinement); 
                attack_counter++;
            }
            else if(release_counter <= bits_to_release)
            {
                refinement = 1 - (static_cast<float>(release_counter)/static_cast<float>(bits_to_release));
                refinement = refinement > 1.0? 1.0 : refinement;
                compress(sample, refinement); 
                release_counter++;
            }
            else
            {
                apply_compression = false;                 
            }

        }
    } 
}

