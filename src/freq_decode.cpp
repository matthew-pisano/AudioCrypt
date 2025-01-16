//
// Created by matthew on 12/8/24.
//

#include "freq_decode.h"

#include <fftw3.h>
#include <vector>
#include <cmath>
#include <iostream>


std::map<double, std::vector<double>> isolateFrequencies(const sf::SoundBuffer& buffer, const std::vector<double>& targetFrequencies,
                        const unsigned int windowSize, const unsigned int overlap, unsigned int paddedSize) {

    if (paddedSize == 0) paddedSize = windowSize;
    if (paddedSize < windowSize) throw std::runtime_error("Padded size must be at least the window size.");

    const sf::Int16* samples = buffer.getSamples();
    const std::size_t sampleCount = buffer.getSampleCount();
    const unsigned int sampleRate = buffer.getSampleRate();

    // Normalize the samples
    std::vector<double> normalizedSamples(sampleCount);
    for (std::size_t i = 0; i < sampleCount; ++i)
        normalizedSamples[i] = static_cast<double>(samples[i]) / 32768.0; // Normalize to [-1, 1]

    // Allocate FFTW input and output
    fftw_complex* out = fftw_alloc_complex(paddedSize / 2 + 1);
    double* in = fftw_alloc_real(paddedSize);

    // Create FFTW plan
    const fftw_plan plan = fftw_plan_dft_r2c_1d(windowSize, in, out, FFTW_ESTIMATE);

    // Map to store amplitudes over time for each target frequency
    std::map<double, std::vector<double>> frequencyAmplitudes;
    for (double freq : targetFrequencies)
        frequencyAmplitudes[freq] = {};

    // Sliding window FFT
    const unsigned int stepSize = windowSize - overlap;
    for (std::size_t start = 0; start + windowSize <= sampleCount; start += stepSize) {
        // Copy window samples
        for (std::size_t i = 0; i < paddedSize; ++i)
            if (i < windowSize) in[i] = normalizedSamples[start + i];
            else in[i] = 0.0; // Zero-padding

        // Perform FFT
        fftw_execute(plan);

        // Extract amplitudes for target frequencies
        for (double targetFreq : targetFrequencies) {
            // Find the closest FFT bin
            std::size_t bin = static_cast<std::size_t>((targetFreq * paddedSize) / sampleRate);
            if (bin < windowSize / 2 + 1) {
                double magnitude = std::sqrt(out[bin][0] * out[bin][0] + out[bin][1] * out[bin][1]);
                frequencyAmplitudes[targetFreq].push_back(magnitude);
            }
            else frequencyAmplitudes[targetFreq].push_back(0.0); // Out of range
        }
    }

    // Cleanup
    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);

    // Output the results
    for (const auto& [freq, amplitudes] : frequencyAmplitudes) {
        std::cout << "Frequency: " << freq << " Hz\nAmplitudes over time:\n";
        for (const double amp : amplitudes) std::cout << amp << " ";
        std::cout << "\n\n";
    }

    return frequencyAmplitudes;
}
