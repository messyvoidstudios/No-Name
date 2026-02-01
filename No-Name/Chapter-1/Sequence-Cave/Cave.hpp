#pragma once
#include "../../Misc/Includes.hpp"
#include "../../Misc/Functions.hpp"

struct CaveLayer {
    sf::VertexArray vArr;
    float depth;
};

inline sf::VertexArray layer(float w, float h, sf::Color c) {
    sf::VertexArray l(sf::PrimitiveType::LineStrip);
    int detail = 15;
    int l_detail = detail * 4;

    for (int i = 0; i <= l_detail; ++i) {
        float angle = (static_cast<float>(i) / l_detail) * 2.f * 3.14159f;

        float radX = w / 2.f;
        float radY = h / 2.f;

        float n = rand(-0.05f, 0.05f);
        float s = 0.f;

        if (angle > 0.2f * 3.14159f && angle < 0.8f * 3.14159f) {
            if (rand(0, 1) > 0.85f) s = -rand(h * 0.1f, h * 0.25f);
        }
        else if (angle > 1.2f * 3.14159f && angle < 1.8f * 3.14159f) {
            if (rand(0, 1) > 0.80f) s = -rand(h * 0.15f, h * 0.4f);
        }

        sf::Vector2f vPos(
            std::cos(angle) * radX * (1.0f + n),
            std::sin(angle) * (radY + s) * (1.0f + n)
        );
        l.append(sf::Vertex({ vPos, c }));
    }
    return l;
}