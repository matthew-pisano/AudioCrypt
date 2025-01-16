//
// Created by matthew on 12/8/24.
//

#ifndef FFT_H
#define FFT_H

#include <map>
#include <SFML/Audio.hpp>

std::map<double, std::vector<double>> isolateFrequencies(const sf::SoundBuffer& buffer, const std::vector<double>& targetFrequencies,
                                                         unsigned int windowSize, unsigned int overlap, unsigned int paddedSize = 0);

#endif //FFT_H
