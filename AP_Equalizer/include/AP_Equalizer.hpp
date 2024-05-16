#include "../../AP_Audio_Effect/include/AP_Audio_Effect.hpp"
#include <vector>
#include <complex>
#include <cmath>

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
    public:
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
    void fft(std::vector<std::complex<double>> &x);
    void ifft(std::vector<std::complex<double>> &x);
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


// Helper function to reverse the bits of an index
unsigned int reverseBits(unsigned int x, int n) {
    unsigned int result = 0;
    for (int i = 0; i < n; ++i) {
        if (x & (1 << i))
            result |= 1 << (n - 1 - i);
    }
    return result;
}

// Iterative FFT function
void AP_Equalizer::fft(std::vector<std::complex<double>> &x) {
    const size_t N = x.size();
    const size_t levels = static_cast<size_t>(log2(N));

    // Bit-reversal permutation
    for (size_t i = 0; i < N; ++i) {
        size_t j = reverseBits(i, levels);
        if (i < j)
            std::swap(x[i], x[j]);
    }

    // Cooley-Tukey FFT
    for (size_t len = 2; len <= N; len *= 2) {
        double ang = 2 * PI / len;
        std::complex<double> wlen(cos(ang), sin(ang));
        for (size_t i = 0; i < N; i += len) {
            std::complex<double> w(1);
            for (size_t j = 0; j < len / 2; ++j) {
                std::complex<double> u = x[i + j];
                std::complex<double> v = x[i + j + len / 2] * w;
                x[i + j] = u + v;
                x[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
}

// Iterative IFFT function
void AP_Equalizer::ifft(std::vector<std::complex<double>> &x) {
    // Conjugate the complex numbers
    for (auto &p : x)
        p = std::conj(p);

    // Forward FFT
    fft(x);

    // Conjugate the complex numbers again
    for (auto &p : x) {
        p = std::conj(p);
        p /= x.size();
    }
}

void AP_Equalizer::applyEqBand(const AP_Equalizer_Band& band)
{
    int32_t N = 1024; // Number of samples (power of 2)
    int32_t sample_rate = m_audio.getSampleRate();

    // Step 1: Get the audio data (this is just an example, adjust according to your implementation)
    std::vector<int16_t> audio_data = m_audio.getSamples();

    // Ensure the audio data size is N
    if (audio_data.size() < N) {
        audio_data.resize(N, 0.0);
    } else if (audio_data.size() > N) {
        audio_data.resize(N);
    }

    // Step 2: Convert audio data to complex format
    std::vector<std::complex<double>> complex_data(N);
    for (int i = 0; i < N; ++i) {
        complex_data[i] = std::complex<double>(audio_data[i], 0.0);
    }

    // Step 3: Apply FFT to convert to frequency domain
    fft(complex_data);

    // Step 4: Apply the equalizer band in the frequency domain
    double gain = band.m_gain;
    int16_t frequency = band.m_frequency;
    int16_t Qfactor = band.m_Qfactor;
    EqualizerType eqType = band.m_eqType;

    for (int i = 0; i < N; ++i) {
        double freq = (double)i * sample_rate / N;

        if (eqType == EqualizerType::HIGH_PASS && freq < frequency) {
            complex_data[i] *= gain;
        } else if (eqType == EqualizerType::LOW_PASS && freq > frequency) {
            complex_data[i] *= gain;
        } else if (eqType == EqualizerType::PARAMETRIC) {
            // Example parametric filter adjustment (simplified)
            double bandwidth = frequency / Qfactor;
            if (std::abs(freq - frequency) < bandwidth) {
                complex_data[i] *= gain;
            }
        }
    }

    // Step 5: Apply IFFT to convert back to time domain
    ifft(complex_data);

    // Step 6: Convert back to real audio data
    for (int i = 0; i < N; ++i) {
        audio_data[i] = complex_data[i].real();
    }
}
