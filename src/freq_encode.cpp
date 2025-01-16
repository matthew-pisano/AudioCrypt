//
// Created by matthew on 12/8/24.
//

#include "freq_encode.h"

#include <climits>
#include <SFML/Audio.hpp>
#include <vector>
#include <cmath>
#include <iostream>


void validateFrequencies(const std::vector<std::pair<double, double>>& frequencies) {
    for (auto& [frequency, amplitude] : frequencies) {
        if (frequency <= 0) throw std::runtime_error("Frequency must be positive.");
        if (amplitude < 0) throw std::runtime_error("Amplitude must be non-negative.");
        if (amplitude > 1) throw std::runtime_error("Amplitude must be less than or equal to 1.");
    }
}

// Function to add frequencies to a mono sound buffer
// {frequency (Hz), amplitude}
sf::SoundBuffer addFrequencies(const sf::SoundBuffer& buffer, const std::vector<std::pair<double, double>>& frequencies) {

    validateFrequencies(frequencies);

    const unsigned int sampleRate = buffer.getSampleRate();
    const std::size_t sampleCount = buffer.getSampleCount();

    // Get the original samples (mono buffer)
    const sf::Int16* samples = buffer.getSamples();

    // Create a new buffer to hold the combined samples
    std::vector<sf::Int16> modifiedSamples(sampleCount, 0);

    // Copy the original samples into the new buffer
    for (std::size_t i = 0; i < sampleCount; ++i) modifiedSamples[i] = samples[i];

    // Add the sine waves for the specified frequencies
    for (auto& [frequency, amplitude] : frequencies) {
        for (std::size_t i = 0; i < sampleCount; ++i) {
            const double time = static_cast<double>(i) / sampleRate; // Time in seconds
            const double wave = amplitude * std::sin(2.0 * M_PI * frequency * time);

            // Add the sine wave to the buffer (single channel)
            modifiedSamples[i] += static_cast<sf::Int16>(wave * SHRT_MAX);
        }
    }

    // Create and load the new buffer
    sf::SoundBuffer modifiedBuffer;
    if (!modifiedBuffer.loadFromSamples(modifiedSamples.data(), modifiedSamples.size(), 1, sampleRate))
        throw std::runtime_error("Failed to create the new sound buffer.");

    return modifiedBuffer;
}