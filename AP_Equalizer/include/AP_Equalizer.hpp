#include "../../AP_Audio_Effect/include/AP_Audio_Effect.hpp"
#include <vector>

enum class EqualizerType
{
    HIGH_PASS,
    LOW_PASS,
    PARAMETRIC
};


class AP_Equalizer: public AP_Audio_Effect
{
    /**
     * Sub Class representing one equlizer band
    */
    class AP_Equalizer_Band
    {
    private:
        float   m_gain;
        int16_t m_frequency;
        int16_t m_Qfactor;
        EqualizerType m_eqType;
    public:
        AP_Equalizer_Band(float _gain, int16_t _frequency, int16_t _Qfactor, EqualizerType _eq_type):
        m_gain(_gain), m_frequency(_frequency), m_Qfactor(_Qfactor), m_eqType(_eq_type){}
        ~AP_Equalizer_Band() = default;
    };
private:
    std::vector<AP_Equalizer_Band> m_eq_band_list;
public:
    AP_Equalizer(AP_AudioIO& _audio, int32_t _mix = 0, int32_t _level = 0);
    ~AP_Equalizer();

    void processAudio();

    bool addBand(float _gain, int16_t _frequency, int16_t _Qfactor, EqualizerType _eq_type);

    bool applyEqBand(const AP_Equalizer_Band& band);
};

// AP_Equalizer::AP_Equalizer(/* args */)
// {
// }

// AP_Equalizer::~AP_Equalizer()
// {
// }
