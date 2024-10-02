import numpy as np
import matplotlib.pyplot as plt
import pyaudio
import wave
from scipy.io import wavfile
from scipy.signal import spectrogram

# Function to record audio from microphone
def record_audio(filename, record_seconds=5, sample_rate=44100, chunk=1024, channels=1):
    # Set up the PyAudio object
    audio = pyaudio.PyAudio()

    # Open stream
    stream = audio.open(format=pyaudio.paInt16, channels=channels,
                        rate=sample_rate, input=True,
                        frames_per_buffer=chunk)

    print("Recording started...")

    frames = []

    for i in range(0, int(sample_rate / chunk * record_seconds)):
        data = stream.read(chunk)
        frames.append(data)

    print("Recording stopped.")

    # Stop and close the stream
    stream.stop_stream()
    stream.close()
    audio.terminate()

    # Save audio to WAV file
    wf = wave.open(filename, 'wb')
    wf.setnchannels(channels)
    wf.setsampwidth(audio.get_sample_size(pyaudio.paInt16))
    wf.setframerate(sample_rate)
    wf.writeframes(b''.join(frames))
    wf.close()

# Function to generate and save a spectrogram from a .wav file
def generate_spectrogram(wav_file, output_image):
    # Read the .wav file
    sample_rate, audio_data = wavfile.read(wav_file)

    # Generate the spectrogram
    frequencies, times, Sxx = spectrogram(audio_data, sample_rate)

    # Avoid log10 of zero by adding a small constant to Sxx
    Sxx = Sxx + 1e-10  # Adding a small constant to avoid log(0)

    # Plot the spectrogram
    plt.figure(figsize=(10, 6))
    plt.pcolormesh(times, frequencies, 10 * np.log10(Sxx), shading='gouraud')
    plt.ylabel('Frequency [Hz]')
    plt.xlabel('Time [sec]')
    plt.title('Spectrogram')
    plt.colorbar(label='Power/Frequency (dB/Hz)')

    # Save the spectrogram as an image
    plt.savefig(output_image)
    plt.show()

if __name__ == "__main__":
    # Define file paths
    recorded_audio_file = "recorded_audio.wav"
    spectrogram_image = "spectrogram_output.png"

    # Record audio from the microphone (adjust record_seconds as needed)
    record_audio(recorded_audio_file, record_seconds=5)

    # Generate and save the spectrogram of the recorded audio
    generate_spectrogram(recorded_audio_file, spectrogram_image)
