# Audio In / Out
Wav file Controler, This module is designed to control the reading and writing of the WAV files in the program.
This module is the main module in the program since it reads a certain WAV file, from here it stores all its header content and also the data - that is, the samples.

Reading the file refers to normal WAV files, another thing I came across during the writing is that there are WAV files (for example the files that Cubase exports) which contain a JUNK chunk before the FMT chunk. Therefore the reading refers to two cases -
1. A normal WAV file in which the header contains exactly 44 bytes
2. A WAV file with a normal opening until the 20th byte, from here we get that FMT=JUNK and the amount of bytes that JUNK contains - therefore we will skip them before continuing to read the header.

![image](https://github.com/SappirBo/Audio-Processor/assets/92790326/1091550a-e051-4b07-9df5-45ebddf5e210)

### Resources
1. https://ccrma.stanford.edu/courses/422-winter-2014/projects/WaveFormat/
2. https://www.youtube.com/watch?v=qqjvB_VxMRM&t=38s&ab_channel=AkashMurthy
