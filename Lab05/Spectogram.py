import numpy as np
import matplotlib.pyplot as plt
from scipy.io import wavfile
from scipy.signal import spectrogram

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
    # Input .wav file path and output image path
    input_wav = "CantinaBand3.wav"
    output_image = "spectrogram_output.png"

    # Generate and save the spectrogram
    generate_spectrogram(input_wav, output_image)
