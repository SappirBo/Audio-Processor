#include "AP_Delay.hpp"


AP_Delay::AP_Delay(AudioIO& audio, float feedback ,float delay): m_audio(audio)
{
    setFeedBack(feedback);
    setDelay(delay);
    int32_t no_value{0};
    m_delay_buffer.resize((m_delay/1000.0)* m_audio.getSampleRate(), no_value);
}

AP_Delay::~AP_Delay()
{
}

void AP_Delay::setFeedBack(float f)
{
    m_feedback = f;
}

void AP_Delay::setDelay(float t)
{
    m_delay = t;
}

void AP_Delay::processAudio()
{
    std::vector<int16_t> output_samples;
    for(auto sample: m_audio.getSamples())
    {
        output_samples.push_back(applyDelay(sample));
    }
    
    m_audio.setSamples(output_samples);
}

int16_t AP_Delay::applyDelay(int16_t sample)
{
    float mix{0.1};

    int16_t delayed_sample{m_delay_buffer.at(write_index)};
    float mixed_sample{(0.9-mix)*float(sample)};
    float mixed_delayed_sample{mix * float(delayed_sample)};

    int16_t out_sample{mixed_sample + mixed_delayed_sample};
    std::cout << "out = " << out_sample << "\n";
    
    m_delay_buffer[write_index] = sample + static_cast<int16_t>(m_feedback * delayed_sample);

    write_index = (write_index + 1) % m_delay_buffer.size();

    return out_sample;
}
