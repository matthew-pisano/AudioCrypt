#include <freq_encode.h>
#include <iostream>
#include <SFML/Audio.hpp>

#include "freq_decode.h"
#include "CLI11/CLI11.hpp"

constexpr int SAMPLE_RATE = 44100;

sf::SoundBuffer convertToMono(const sf::SoundBuffer& stereoBuffer) {
    // Ensure the input buffer is stereo
    if (stereoBuffer.getChannelCount() != 2) throw std::runtime_error("Input buffer must have 2 channels.");

    const sf::Int16* stereoSamples = stereoBuffer.getSamples();
    const std::size_t stereoSampleCount = stereoBuffer.getSampleCount();

    // Allocate memory for mono samples
    std::vector<sf::Int16> monoSamples;
    monoSamples.reserve(stereoSampleCount / 2);

    // Mix stereo samples into mono
    for (std::size_t i = 0; i < stereoSampleCount; i += 2) {
        // Average left and right channels
        sf::Int16 monoSample = static_cast<sf::Int16>((stereoSamples[i] + stereoSamples[i + 1]) / 2);
        monoSamples.push_back(monoSample);
    }

    sf::SoundBuffer monoBuffer;

    // Load the mono samples into a new sound buffer
    if (!monoBuffer.loadFromSamples(monoSamples.data(), monoSamples.size(), 1, stereoBuffer.getSampleRate()))
        throw std::runtime_error("Failed to load mono samples.");

    return monoBuffer;
}


std::vector<sf::Int16> samplesFromFile(const std::string& filename) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filename)) throw std::runtime_error("Failed to load file: " + filename);

    if (buffer.getChannelCount() != 1)
        buffer = convertToMono(buffer);

    std::cout << "Loaded file: " << filename << std::endl;
    std::cout << "Sample count: " << buffer.getSampleCount() << std::endl;
    std::cout << "Sample rate: " << buffer.getSampleRate() << std::endl;

    const sf::SoundBuffer modifiedBuffer = addFrequencies(buffer, {{440.0, 0.5}});

    sf::Sound sound;
    sound.setBuffer(modifiedBuffer);
    sound.play();
    sf::sleep(sf::seconds(static_cast<float>(modifiedBuffer.getSampleCount()) / modifiedBuffer.getSampleRate()));
    sound.stop();

    std::vector<sf::Int16> modifiedSamples;
    modifiedSamples.reserve(modifiedBuffer.getSampleCount());
    for (int i = 0; i < modifiedBuffer.getSampleCount(); i++) {
        sf::Int16 sample = modifiedBuffer.getSamples()[i];
        modifiedSamples.push_back(sample);
    }

    return modifiedSamples;
}


std::vector<sf::Int16> samplesFromMicrophone(const int sampleCount, const int sampleRate) {
    sf::SoundBufferRecorder recorder;
    recorder.setChannelCount(1);
    recorder.start(sampleRate);

    std::cout << "Recording..." << std::endl;
    std::cout << "Sample count: " << sampleCount << std::endl;
    std::cout << "Sample rate: " << sampleRate << std::endl;

    sf::sleep(sf::seconds(static_cast<float>(sampleCount) / sampleRate));
    recorder.stop();

    const sf::SoundBuffer& buffer = recorder.getBuffer();
    std::vector<sf::Int16> samples;
    samples.reserve(buffer.getSampleCount());
    for (int i = 0; i < buffer.getSampleCount(); i++) {
        const sf::Int16 sample = buffer.getSamples()[i];
        samples.push_back(sample);
    }

    isolateFrequencies(buffer, {440.0}, 4096, 2048);

    return samples;
}

int main(const int argc, char** argv) {

    if (argc == 1) {
        std::cerr << "No arguments provided." << std::endl;
        return 1;
    }

    std::vector<sf::Int16> samples;
    if (argv[1] == std::string("record") && argc == 3) {
        const int seconds = std::stoi(argv[2]);
        samples = samplesFromMicrophone(SAMPLE_RATE*seconds, SAMPLE_RATE);
    }
    else if (argv[1] == std::string("file") && argc == 3)
        samples = samplesFromFile(argv[2]);
    else {
        std::cerr << "Invalid argument(s): " << argv[1] << std::endl;
        return 1;
    }

    // for (const sf::Int16 sample : samples) std::cout << sample << std::endl;

    return 0;
}