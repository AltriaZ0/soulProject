#pragma once
#include <SFML/System/Angle.hpp>

namespace Config {
    constexpr unsigned int WindowWidth = 800;
    constexpr unsigned int WindowHeight = 900;
}

inline sf::Angle degrees(float deg) {
    return sf::degrees(deg);
}

inline sf::Angle radians(float rad) {
    return sf::radians(rad);
}