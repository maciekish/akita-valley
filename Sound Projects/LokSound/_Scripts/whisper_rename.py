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
    return re.sub(r'\.+', ' ', spaces).strip()

def convert_to_whisper_compatible(file_path):
    temp_wav = tempfile.NamedTemporaryFile(delete=False, suffix=".wav")
    temp_wav_path = temp_wav.name
    temp_wav.close()

    subprocess.run([
        "ffmpeg",
        "-y",
        "-i", file_path,
        "-ar", "16000",
        "-ac", "1",
        "-c:a", "pcm_s16le",
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
    converted_path = convert_to_whisper_compatible(file_path)
    result = model.transcribe(converted_path, language=language)
    os.remove(converted_path)
    return result

def convert_romaji_only(text):
    kakasi_inst = kakasi()
    kakasi_inst.setMode("H", "a")       # Convert Hiragana to ascii
    kakasi_inst.setMode("K", "a")       # Convert Katakana to ascii
    kakasi_inst.setMode("J", "a")       # Convert Kanji to ascii
    kakasi_inst.setMode("r", "Hepburn") # Use Hepburn romanization
    kakasi_inst.setMode("s", True)      # Add space (split words)
    conv = kakasi_inst.getConverter()
    return conv.do(text)

def translate_to_english(text, src_lang):
    translator = Translator()
    return translator.translate(text, src=src_lang, dest="en").text

def sanitize_filename(text):
    filename = text.strip().strip(". ") \
        .replace("/", "-") \
        .replace("\\", "-") \
        .replace(":", "-") \
        .replace("?", "") \
        .replace("。", ".") \
        .replace("_", " ") \
        .replace("、", ",")
    return collapse_spaces(filename)

class StickyTqdm:
    def __init__(self, iterable, **kwargs):
        self.progress = tqdm(iterable, **kwargs)

    def write_above(self, text):
        self.progress.clear()
        sys.stdout.write(text + "\n")
        sys.stdout.flush()
        self.progress.refresh()

    def __iter__(self):
        return self._wrap_iter()

    def _wrap_iter(self):
        for item in self.progress:
            yield item
        self.progress.close()

def process_folder(folder_path):
    all_wav_files = []
    for root, dirs, files in os.walk(folder_path):
        for filename in files:
            if filename.lower().endswith(".wav"):
                all_wav_files.append(os.path.join(root, filename))

    total_files = len(all_wav_files)
    if total_files == 0:
        print("No .wav files found.")
        return

    print(f"Found {total_files} .wav files. Starting transcription...\n")

    sticky_bar = StickyTqdm(
        all_wav_files,
        desc="Processing",
        unit="file",
        dynamic_ncols=True,
        bar_format="{l_bar}{bar}| {n_fmt}/{total_fmt} [{elapsed}<{remaining}, {rate_fmt}]"
    )

    for file_path in sticky_bar:
        filename = os.path.basename(file_path)
        try:
            detected_lang = detect_language(file_path)
            # CHANGED: Force only 'en' or 'ja'. If not 'en', use 'ja'.
            if detected_lang == "en":
                sticky_bar.write_above(f"Detected language: en")
                lang = "en"
            else:
                sticky_bar.write_above(f"Detected {detected_lang} - forcing 'ja'")
                lang = "ja"

            result = transcribe(file_path, language=lang)
            text = result["text"].strip()
        except Exception as e:
            sticky_bar.write_above(f"Failed to transcribe {filename}: {e}")
            continue

        if not text:
            sticky_bar.write_above("Empty transcription.")
            continue

        english = ""
        if lang != "en":
            # If language is "ja", attempt to translate to English
            try:
                english = translate_to_english(text, src_lang=lang)
                english = sanitize_filename(english)
            except Exception as e:
                sticky_bar.write_above(f"Translation failed: {e}")

        special_prefix = get_special_prefix(filename)

        if lang == "ja":
            romaji = convert_romaji_only(text).replace(" ", "_")
            romaji = sanitize_filename(romaji)
            new_name = f"{special_prefix}{romaji} ({english}).wav" if english else f"{special_prefix}{romaji}.wav"
        else:
            # English source
            source = sanitize_filename(text.replace(" ", "_"))
            new_name = f"{special_prefix}{source}.wav"

        MAX_FILENAME_LENGTH = 250
        base, ext = os.path.splitext(new_name)
        if len(base) > MAX_FILENAME_LENGTH:
            base = base[:MAX_FILENAME_LENGTH].rstrip()
            new_name = f"{base}{ext}"

        new_path = os.path.join(os.path.dirname(file_path), new_name)

        if len(new_path.encode('utf-8')) > 255:
            sticky_bar.write_above("Warning: final path too long, truncating further...")
            base = base[:200]
            new_name = f"{base}{ext}"
            new_path = os.path.join(os.path.dirname(file_path), new_name)

        # Keep incrementing filename suffix if a file exists
        try:
            os.rename(file_path, new_path)
            sticky_bar.write_above(f"Renamed to: {new_name}")
        except FileExistsError:
            suffix_number = 2
            while True:
                new_name_try = f"{base} ({suffix_number}){ext}"
                new_path_try = os.path.join(os.path.dirname(file_path), new_name_try)
                try:
                    os.rename(file_path, new_path_try)
                    sticky_bar.write_above(f"File existed. Renamed to: {new_name_try}")
                    break
                except FileExistsError:
                    suffix_number += 1
                    # Keep looping until successful

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python whisper_translate_all.py /path/to/folder")
        sys.exit(1)
    process_folder(sys.argv[1])
