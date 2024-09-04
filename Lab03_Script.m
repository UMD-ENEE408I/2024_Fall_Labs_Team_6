% ENEE408I Lab 03

% Part A: Analyzing Audio Signal and Downsampling

% Read the audio file
[audioSignal, Fs] = audioread('human_voice.wav');
% Display the original sampling frequency
fprintf('Original Sampling Frequency: %d Hz\n', Fs);

% Plot the original audio signal
time = (0:length(audioSignal)-1) / Fs;
figure;
plot(time, audioSignal);
title('Original Audio Signal');
xlabel('Time (s)');
ylabel('Amplitude');


% Downsample the signal to 8 kHz
targetFs = 8000;
downsampleRatio = round(Fs / targetFs);
downsampledSignal = audioSignal(1:downsampleRatio:end);

% New number of samples after downsampling
newSamples = length(downsampledSignal);
fprintf('Number of samples after downsampling: %d\n', newSamples);

% Plot the downsampled audio signal
downsampledTime = (0:length(downsampledSignal)-1) / targetFs;
figure;
plot(downsampledTime, downsampledSignal);
title('Downsampled Audio Signal');
xlabel('Time (s)');
ylabel('Amplitude');

%%
%  Part B: RMS, Cross-correlation, Sound Localization
% Read audio signals from microphones
[m1, fs1] = audioread('M1.wav');
[m2, fs2] = audioread('M2.wav');
[m3, fs3] = audioread('M3.wav');

% Calculate RMS values
rms_m1 = sqrt(mean(m1.^2));
rms_m2 = sqrt(mean(m2.^2));
rms_m3 = sqrt(mean(m3.^2));

fprintf('RMS value of M1: %.2f\n', rms_m1);
fprintf('RMS value of M2: %.2f\n', rms_m2);
fprintf('RMS value of M3: %.2f\n', rms_m3);

% Manual Cross-Correlation between M1 and M2
len = length(m1) + length(m2) - 1;
crossCorr = zeros(len, 1);
for m = 1:len
    for n = 1:length(m1)
        if (m - n + 1 > 0 && m - n + 1 <= length(m2))
            crossCorr(m) = crossCorr(m) + m1(n) * m2(m - n + 1);
        end
    end
end

% Find time delay
[~, delayIndex] = max(crossCorr);
timeDelay = (delayIndex - length(m1)) / fs1;
fprintf('Time delay between M1 and M2: %.4f seconds\n', timeDelay);


% Calculate angle based on time delay
r = 0.1; % Radius of the robot (10 cm)
speedOfSound = 343; % Speed of sound in air (m/s)
d1 = timeDelay * speedOfSound / 2;
theta = acos(d1 / r) * (180 / pi); % Convert to degrees

fprintf('The angle θ to correct heading: %.2f degrees\n', theta);

%%
%Part C: Frequency Spectrum
% Read the noisy audio signal
[cafeSignal, Fs] = audioread('Cafe_with_noise.wav');

% Plot the noisy signal
time = (0:length(cafeSignal)-1) / Fs;
figure;
plot(time, cafeSignal);
title('Noisy Audio Signal');
xlabel('Time (s)');
ylabel('Amplitude');

% Compute FFT of the noisy signal
N = length(cafeSignal);
cafeFFT = fft(cafeSignal);
freq = (0:N-1) * (Fs / N);

% Plot the magnitude of the FFT
figure;
plot(freq, abs(cafeFFT));
title('Frequency Spectrum of Noisy Audio Signal');
xlabel('Frequency (Hz)');
ylabel('Magnitude');
xlim([0, Fs/2]);

% Design a low-pass filter (e.g., using Butterworth filter)
cutoffFreq = 1000; % Cutoff frequency of 1 kHz
[b, a] = butter(4, cutoffFreq / (Fs / 2), 'low'); % 4th-order Butterworth filter

% Apply the filter
filteredSignal = filter(b, a, cafeSignal);

% Plot the filtered signal
figure;
plot(time, filteredSignal);
title('Filtered Audio Signal');
xlabel('Time (s)');
ylabel('Amplitude');
