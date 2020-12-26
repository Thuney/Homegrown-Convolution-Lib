import wave

def get_samples_from_wav(input_file_path):

    wav_reader   = wave.open(input_file_path, "r")
    sample_width = wav_reader.getsampwidth()

    wav_bytes = wav_reader.readframes(-1)

    samples = []

    for i in range(0, len(wav_bytes), sample_width):

        sample_data_as_int   = (int.from_bytes(wav_bytes[i: i + sample_width], "little", signed=True))
        sample_data_as_float = (float(sample_data_as_int) / (2 ** ((8*sample_width) - 1)))

        samples.append(sample_data_as_float)

    return samples

def signal_to_wav(signal, wav_file_path):

    signal_wav = wave.open(wav_file_path, "wb")
    signal_wav.setnchannels(1)
    signal_wav.setsampwidth(2)
    signal_wav.setframerate(44100)

    for sample in signal:

        sample_to_scaled_wav_data = (int(sample * (2 ** 15))).to_bytes(2, byteorder="little", signed=True)
        signal_wav.writeframesraw(sample_to_scaled_wav_data)

    signal_wav.close()
