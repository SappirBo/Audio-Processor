import wave
import struct
from PathList import PathList

# Load the WAV file to inspect its headers
# file_path = PathList.path_list[0]
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
    
    size_of_one_frame = int(header_info['Number of Channels']) * (int(header_info['Bits Per Sample']) /8)

    total_number_of_frames = int(header_info['data Size']) / size_of_one_frame

    header_info['Number of Frames'] = total_number_of_frames
    
    return header_info


def interpret_wav_data(header_info):
    # Create a dictionary to hold the interpreted data
    interpreted_data = {}

    if 'Sample Rate' in header_info and 'Number of Frames' in header_info:
        # Calculate total duration of the audio track
        total_seconds = header_info['Number of Frames'] / header_info['Sample Rate']
        interpreted_data['Total Duration (seconds)'] = total_seconds

        # Format the duration into a more readable format (hours:minutes:seconds)
        # hours = int(total_seconds // 3600)
        minutes = int((total_seconds % 3600) // 60)
        seconds = total_seconds % 60
        interpreted_data['Readable Duration'] = f"{minutes}m:{seconds:.2f}s"

    if 'Byte Rate' in header_info:
        # Calculate the total data size in bytes (excludes headers)
        if 'data Size' in header_info:
            total_data_size_bytes = header_info['data Size']
            interpreted_data['Total Data Size (bytes)'] = total_data_size_bytes

    if 'Number of Channels' in header_info:
        # State the type of audio (Mono/Stereo/Multichannel)
        if header_info['Number of Channels'] == 1:
            interpreted_data['Audio Type'] = 'Mono'
        elif header_info['Number of Channels'] == 2:
            interpreted_data['Audio Type'] = 'Stereo'
        else:
            interpreted_data['Audio Type'] = f"Multichannel ({header_info['Number of Channels']} channels)"

    return interpreted_data


# Read and display the WAV file header properties
header_settings = read_wav_header(file_path)


print('- - - - - - Header Information: - - - - - -')
for header_setting, value in header_settings.items():
    print('     ',header_setting , ': ', value)

print('- - - - - - Interpret Information: - - - - - -')
wav_data = interpret_wav_data(header_settings)

for name, data in wav_data.items():
    print('     ',name, ': ', data)