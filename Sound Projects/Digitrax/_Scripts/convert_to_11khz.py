#!/usr/bin/env python3

import os
import sys
from pydub import AudioSegment

def convert_wav_files(directory):
    """Convert all .wav files in the directory to 11kHz 16-bit PCM format."""
    for filename in os.listdir(directory):
        if filename.lower().endswith(".wav"):
            file_path = os.path.join(directory, filename)
            print(f"Processing: {file_path}")

            try:
                # Load the audio file
                audio = AudioSegment.from_wav(file_path)

                # Convert to 11kHz, 16-bit PCM
                converted_audio = audio.set_frame_rate(11025).set_sample_width(2)

                # Overwrite the file
                converted_audio.export(file_path, format="wav", parameters=["-acodec", "pcm_s16le"])
                print(f"Converted: {file_path}")
            except Exception as e:
                print(f"Error processing {file_path}: {e}")

if __name__ == "__main__":
    # Use the current directory or a provided directory
    target_directory = sys.argv[1] if len(sys.argv) > 1 else os.getcwd()
    convert_wav_files(target_directory)
