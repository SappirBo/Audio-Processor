# Compressor
If we want to increase the gain of an audio segment, we will encounter a problem - amplification without recognition may lead the signal to reach its peak - where it will get stuck and sound bad (we will get a square sound wave), such amplification of the signal is called "Break Wall Limiting" (the signal limited to a finite number of values without paying attention to the distortion that happens to it).

A compressor allows us to control the amplitude of the signal in a more musical and automated way. Each compressor has four main parts:
1. **Threshold:** This is the level at which the compressor starts to reduce the volume of the audio. Any signal that exceeds this level will be compressed.
2. **Ratio:** This sets how much the signal will be reduced once it crosses the threshold. For example, a ratio of 4:1 means that if the input level is 4 dB over the threshold, the output level will be 1 dB over the threshold.
3. **Attack:** This is the time it takes for the compressor to start working after the signal exceeds the threshold. A faster attack time means the compressor reacts more quickly.
4. **Release:** This is how long the compressor continues to affect the audio after the signal falls below the threshold. A shorter release time means the compressor stops affecting the audio more quickly.




<br/>

![image](https://github.com/SappirBo/Audio-Processor/assets/92790326/67180a62-4629-42fd-a114-126c2b499f57)
