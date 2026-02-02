#pragma once
#include "../../../Misc/Includes.hpp"

struct Dust {
    sf::Vector2f pos;
    float depth;
    float spd;
    float offset;
};

inline std::vector<Dust> dust;

inline void initDust() {
    for (int i = 0; i < 100; ++i) {
        dust.push_back({ {rand(-800, 800), rand(-500, 500)}, rand(0, 10), rand(1.2f, 2.0f), rand(0, 6.28f) });
    }
}