//
// Created by matthew on 12/8/24.
//

#ifndef FREQ_ENCODE_H
#define FREQ_ENCODE_H

#include <SFML/Audio.hpp>

sf::SoundBuffer addFrequencies(const sf::SoundBuffer& buffer, const std::vector<std::pair<double, double>>& frequencies);

#endif //FREQ_ENCODE_H
