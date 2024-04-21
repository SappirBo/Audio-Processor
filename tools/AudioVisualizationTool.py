import wave
import numpy as np
import matplotlib.pyplot as plt
from PathList import PathList

# Load WAV file
wav_obj = wave.open(PathList.path_to_output, 'rb')
# wav_obj = wave.open(PathList.path_list[0], 'rb')

# Get audio properties
sample_freq = wav_obj.getframerate()
n_samples = wav_obj.getnframes()
t_audio = n_samples / sample_freq  # Total duration of audio

# Read the waveform data
signal_wave = wav_obj.readframes(n_samples)
signal_array = np.frombuffer(signal_wave, dtype=np.int16)

# Split stereo into two channels
l_channel = signal_array[0::2]  # Left channel

# Create time array for one channel only
times = np.linspace(0, t_audio, num=len(l_channel))  # Adjust number of points in times

# Plotting
plt.figure(figsize=(15, 5))
plt.plot(times, l_channel)
plt.title('Channel')
plt.ylabel('Signal Value')
plt.xlabel('Time (s)')
plt.xlim(0, t_audio)

# Save the figure
plt.savefig('/home/sappirb/code/Audio-Processor/tools/tools_output/SignalAmplitudeVisual.png')
plt.close()

plt.clf()

plt.figure(figsize=(15, 5))
plt.specgram(l_channel, Fs=sample_freq, vmin=-20, vmax=50)
plt.title('Channel')
plt.ylabel('Frequency (Hz)')
plt.xlabel('Time (s)')
plt.xlim(0, t_audio)

# Save the figure
plt.savefig('/home/sappirb/code/Audio-Processor/tools/tools_output/FrequencySpectrumVisual.png')
plt.close()


print('End, Out ...')
