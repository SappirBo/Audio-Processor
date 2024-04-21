# ifndef AP_AUDIO_EFFECT_HPP
# define AP_AUDIO_EFFECT_HPP

#include "AP_AudioIO.hpp"
#include <cstdint>
#include <iostream>


class AP_Audio_Effect
{
public:
    float m_mix;
    int32_t m_level;
    AP_AudioIO& m_audio;

    // Constructor (optional, depending on whether you need to initialize m_mix)
    explicit AP_Audio_Effect(AP_AudioIO& audio, int32_t mix = 0, int32_t level = 0):
    m_audio(audio)
    {
        setMix(mix);
        setLevel(level);
    }

    void setMix(int32_t mix = 0)
    {
        mix = (mix>100?100: (mix<0?0:mix));
        m_mix = float(mix)/100;   
    }

    // Declare a pure virtual function for processing audio
    virtual void processAudio() = 0;

    // Virtual destructor to ensure proper cleanup of derived classes
    virtual ~AP_Audio_Effect() {}
    
    int16_t applyLevel(int16_t sample) {    
        float sample_scaled{float(sample) / INT16_MAX};
        float level_scaled{(float(m_level)/100)};
        float sample_times_leveling{sample_scaled * level_scaled};
        int16_t sample_leveled = int16_t(float(INT16_MAX) * sample_times_leveling) ;
        return sample_leveled ;
    }

    /** Set the Level -- 0 - 100 */
    void setLevel(int16_t level)
    {
        level = (level>100? 100: (level<0? 0: level));
        m_level = level;
    }
};

#endif