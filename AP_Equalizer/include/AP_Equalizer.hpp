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
        AP_Equalizer_Band(float gain, int16_t frequency, int16_t Qfactor, EqualizerType eq_type):
        m_gain(gain), m_frequency(frequency), m_Qfactor(Qfactor), m_eqType(eq_type){}
        ~AP_Equalizer_Band() = default;
    };
private:
    std::vector<AP_Equalizer_Band> m_eq_band_list;
public:
    AP_Equalizer(AP_AudioIO& audio, int32_t mix = 0, int32_t level = 0);
    ~AP_Equalizer();

    void processAudio();

    void addBand(float gain, int16_t frequency, int16_t Qfactor, EqualizerType eq_type);

    void applyEqBand(const AP_Equalizer_Band& band);
};

AP_Equalizer::AP_Equalizer(AP_AudioIO& audio, int32_t mix, int32_t level):
AP_Audio_Effect(audio,mix,level){}

AP_Equalizer::~AP_Equalizer(){}

void AP_Equalizer::addBand(float gain, int16_t frequency, int16_t Qfactor, EqualizerType eq_type)
{
    AP_Equalizer_Band new_band{gain, frequency, Qfactor, eq_type};
    m_eq_band_list.push_back(new_band);
}

void AP_Equalizer::processAudio()
{
    for(auto& band: m_eq_band_list)
    {
        applyEqBand(band);
    }
}

void AP_Equalizer::applyEqBand(const AP_Equalizer_Band& band)
{
    int32_t N{1024};
    int32_t sample_rate{m_audio.getSampleRate()};
}
