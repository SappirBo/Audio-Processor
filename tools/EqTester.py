import wave
import numpy as np
from scipy.signal import butter, lfilter

# Function to apply a filter
def apply_filter(data, cutoff, fs, btype='low', order=5):
    nyquist = 0.5 * fs
    if isinstance(cutoff, list):  # Check if it's a band-pass or band-stop filter
        normal_cutoff = [c / nyquist for c in cutoff]
    else:
        normal_cutoff = cutoff / nyquist

    b, a = butter(order, normal_cutoff, btype=btype, analog=False)
    return lfilter(b, a, data)

# Function to read a WAV file using the wave module
def read_wav_file(file_path):
    with wave.open(file_path, 'r') as wav:
        params = wav.getparams()
        n_channels, sample_width, sample_rate, n_frames = params[:4]
        frames = wav.readframes(n_frames)
        if sample_width == 2:
            dtype = np.int16
        else:
            raise ValueError("Unsupported sample width")
        
        data = np.frombuffer(frames, dtype=dtype)
        # Handle stereo by taking the first channel only (interleaved samples)
        if n_channels == 2:
            data = data[::2]
        
        return sample_rate, data

# Function to write a WAV file using the wave module
def write_wav_file(file_path, data, sample_rate):
    n_channels = 1
    sample_width = 2  # 16-bit audio
    with wave.open(file_path, 'w') as wav:
        wav.setnchannels(n_channels)
        wav.setsampwidth(sample_width)
        wav.setframerate(sample_rate)
        wav.writeframes(data.tobytes())

# Function to apply the equalizer
def equalizer(input_path, output_path, gains):
    # Read WAV file
    sample_rate, data = read_wav_file(input_path)

    # Normalize to float values
    data = data / np.max(np.abs(data))

    # Set frequency cutoffs
    low_cutoff = 300
    high_cutoff = 3000

    # Apply low-pass filter for the low frequencies
    low_filtered = apply_filter(data, low_cutoff, sample_rate, btype='low')
    low_filtered *= gains['low']

    # Apply band-pass filter for mid frequencies
    mid_filtered = apply_filter(data, [low_cutoff, high_cutoff], sample_rate, btype='band')
    mid_filtered *= gains['mid']

    # Apply high-pass filter for high frequencies
    high_filtered = apply_filter(data, high_cutoff, sample_rate, btype='high')
    high_filtered *= gains['high']

    # Combine all bands
    processed_data = low_filtered + mid_filtered + high_filtered

    # Normalize back to original range and convert to integer
    processed_data = np.int16(processed_data / np.max(np.abs(processed_data)) * 32767)

    # Write back to a new WAV file
    write_wav_file(output_path, processed_data, sample_rate)

# Example usage
gains = {
    'low': 3.0,   # Gain for low frequencies
    'mid': 1.0,   # Gain for mid frequencies
    'high': 5.0   # Gain for high frequencies
}

equalizer('/content/Audio_Processor_Drums.wav', '/content/input.wav', gains)
