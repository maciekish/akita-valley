#!/Users/maciekish/.pyenv/shims/python3

# Requirements
# pip install pykakasi googletrans==4.0.0-rc1 tqdm
# pip install git+https://github.com/openai/whisper.git

# Silence FP16 CPU warning from Whisper and pykakasi deprecation warnings
import warnings
warnings.filterwarnings("ignore", message="FP16 is not supported on CPU")
warnings.simplefilter("ignore", category=DeprecationWarning)

import os
import sys
import re
import whisper
import tempfile
import subprocess
from tqdm import tqdm
from pykakasi import kakasi
from googletrans import Translator

model = whisper.load_model("large")  # highest accuracy

# Define special filename handling
special_prefix_map = {
    "_m.": "m_",
    "_m_": "m_",
    "m_": "m_",
    "_atos_": "atos_",
    "atos_": "atos_",
    "_auto_": "auto_",
    "auto_": "auto_",
    "_shasho_": "shasho_",
    "shasho_": "shasho_"
}

def get_special_prefix(filename):
    for key, prefix in special_prefix_map.items():
        if key in filename:
            return prefix
    return ""

def collapse_spaces(text):
    spaces = re.sub(r'\s+', ' ', text).strip()
    return re.sub(r'\.+', ' ', text).strip()

def convert_to_whisper_compatible(file_path):
    temp_wav = tempfile.NamedTemporaryFile(delete=False, suffix=".wav")
    temp_wav_path = temp_wav.name
    temp_wav.close()

    subprocess.run([
        "ffmpeg",
        "-y",  # Overwrite if exists
        "-i", file_path,
        "-ar", "16000",     # Sample rate
        "-ac", "1",         # Mono
        "-c:a", "pcm_s16le",  # 16-bit PCM
        temp_wav_path
    ], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

    return temp_wav_path

def detect_language(file_path):
    converted_path = convert_to_whisper_compatible(file_path)
    audio = whisper.load_audio(converted_path)
    audio = whisper.pad_or_trim(audio)
    mel = whisper.log_mel_spectrogram(audio, model.dims.n_mels).to(model.device)
    os.remove(converted_path)
    _, probs = model.detect_language(mel)
    detected_lang = max(probs, key=probs.get)
    return detected_lang

def transcribe(file_path, language=None):
    print(f"Transcribing: {os.path.basename(file_path)}")
    converted_path = convert_to_whisper_compatible(file_path)
    result = model.transcribe(converted_path, language=language)
    os.remove(converted_path)
    return result

def convert_romaji_only(text):
    kakasi_inst = kakasi()
    kakasi_inst.setMode("H", "a")
    kakasi_inst.setMode("K", "a")
    kakasi_inst.setMode("J", "a")
    kakasi_inst.setMode("r", "Hepburn")
    kakasi_inst.setMode("s", True)
    conv = kakasi_inst.getConverter()
    return conv.do(text)

def translate_to_english(text, src_lang):
    translator = Translator()
    return translator.translate(text, src=src_lang, dest="en").text

def sanitize_filename(text):
    filename = text.strip().strip(". ").replace("/", "-").replace("\\", "-").replace(":", "-").replace("?", "").replace("。", ".").replace("_", " ").replace("、", ",")
    return collapse_spaces(filename)

def process_folder(folder_path):
    # First, count all .wav files
    all_wav_files = []
    for root, dirs, files in os.walk(folder_path):
        for filename in files:
            if filename.lower().endswith(".wav"):
                all_wav_files.append(os.path.join(root, filename))

    total_files = len(all_wav_files)
    if total_files == 0:
        print("No .wav files found.")
        return

    print(f"Found {total_files} .wav files. Starting transcription...")

    for file_path in tqdm(
        all_wav_files,
        desc="Processing",
        unit="file",
        dynamic_ncols=True,
        bar_format="{l_bar}{bar}| {n_fmt}/{total_fmt} [{elapsed}<{remaining}, {rate_fmt}]"
    ):
        filename = os.path.basename(file_path)
        try:
            lang = detect_language(file_path)
            print(f"\nDetected language: {lang}")
            result = transcribe(file_path, language=lang)
            text = result["text"].strip()
        except Exception as e:
            print(f"\nFailed to transcribe {filename}: {e}")
            continue

        if not text:
            print("\nEmpty transcription.")
            continue

        english = ""
        if lang != "en":
            try:
                english = translate_to_english(text, src_lang=lang)
                english = sanitize_filename(english)
            except Exception as e:
                print(f"\nTranslation failed: {e}")

        special_prefix = get_special_prefix(filename)

        if lang == "ja":
            romaji = convert_romaji_only(text).replace(" ", "_")
            romaji = sanitize_filename(romaji)
            new_name = f"{special_prefix}{romaji} ({english}).wav" if english else f"{special_prefix}{romaji}.wav"
        else:
            source = sanitize_filename(text.replace(" ", "_"))
            new_name = f"{special_prefix}{source}.wav"

        MAX_FILENAME_LENGTH = 250
        base, ext = os.path.splitext(new_name)
        if len(base) > MAX_FILENAME_LENGTH:
            base = base[:MAX_FILENAME_LENGTH].rstrip()
            new_name = f"{base}{ext}"

        new_path = os.path.join(os.path.dirname(file_path), new_name)

        if len(new_path.encode('utf-8')) > 255:
            print("\nWarning: final path too long, truncating further...")
            base = base[:200]
            new_name = f"{base}{ext}"
            new_path = os.path.join(os.path.dirname(file_path), new_name)

        os.rename(file_path, new_path)
        print(f"\nRenamed to: {new_name}")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python whisper_translate_all.py /path/to/folder")
        sys.exit(1)
    process_folder(sys.argv[1])
