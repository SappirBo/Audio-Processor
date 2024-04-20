import wave
import struct
from PathList import PathList

# Load the WAV file to inspect its headers
file_path = PathList.path_to_output


def read_wav_header(file_path):
    header_info = {}
    
    with open(file_path, 'rb') as file:
        # Read the basic RIFF header
        riff, size, wave = struct.unpack('<4sI4s', file.read(12))
        header_info['RIFF header'] = riff.decode()
        header_info['File size'] = size
        header_info['WAVE header'] = wave.decode()

        # Read chunks in the file header until 'data' chunk is found
        while True:
            chunk_header = file.read(8)
            if not chunk_header:
                break
            
            chunk_id, chunk_size = struct.unpack('<4sI', chunk_header)
            chunk_id = chunk_id.decode()

            if chunk_id == 'fmt ':
                fmt_data = file.read(chunk_size)
                audio_format, num_channels, sample_rate, byte_rate, block_align, bits_per_sample = struct.unpack('<HHIIHH', fmt_data)
                header_info['fmt chunk ID'] = chunk_id
                header_info['Audio Format'] = audio_format
                header_info['Number of Channels'] = num_channels
                header_info['Sample Rate'] = sample_rate
                header_info['Byte Rate'] = byte_rate
                header_info['Block Align'] = block_align
                header_info['Bits Per Sample'] = bits_per_sample
            elif chunk_id == 'data':
                header_info['data chunk ID'] = chunk_id
                header_info['data Size'] = chunk_size
                break  # Stop after reading the 'data' chunk header
            else:
                # If the chunk is neither 'fmt ' nor 'data', skip the data of this chunk
                file.seek(chunk_size, 1)  # Move the file pointer to skip the size of the chunk
                header_info[chunk_id + ' chunk Size'] = chunk_size

    return header_info

# Read and display the WAV file header properties
header_settings = read_wav_header(file_path)

for header_setting, value in header_settings.items():
    print(header_setting , ': ', value)