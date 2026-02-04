#pragma once
#include "../../../Misc/Includes.hpp"

struct Dust {
    sf::Vector2f pos;
    float depth;
    float spd;
    float offset;
};

inline std::vector<Dust> dust;

inline void uDust(float et, sf::Vector2f bobOffset, sf::RenderWindow& window) {
    sf::Vector2f centre(window.getSize().x / 2.f, window.getSize().y / 2.f);
    sf::VertexArray dustParticles(sf::PrimitiveType::Points);
    for (const auto& d : dust) {
        float s = std::pow(0.75f, d.depth);

        float sinX = std::sin(et + d.offset) * 20.f;
        float cosY = std::cos(et * 0.7f + d.offset) * 15.f;

        sf::Vector2f fPos = centre + bobOffset + ((d.pos + sf::Vector2f(sinX, cosY)) * s);

        float a = std::clamp(d.depth / 2.0f, 0.0f, 0.5f);
        dustParticles.append(sf::Vertex({ fPos, sf::Color(200, 200, 200, (uint8_t)(255 * a)) }));
    }
    window.draw(dustParticles);
}

inline void initDust() {
    for (int i = 0; i < 100; ++i) {
        dust.push_back({ {rand(-800, 800), rand(-500, 500)}, rand(0, 10), rand(1.2f, 2.0f), rand(0, 6.28f) });
    }
}