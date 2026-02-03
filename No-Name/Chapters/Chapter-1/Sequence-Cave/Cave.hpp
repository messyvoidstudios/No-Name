#pragma once
#include "../../../Misc/Includes.hpp"
#include "../../../Misc/Functions.hpp"
#include "../../../Misc/Variables.hpp"

#include "../../../UI/Text.hpp"

struct CLayer {
    sf::VertexArray vArr;
    float depth;
};

struct CMilestone {
    std::string text;
    float duration;
    bool triggered = false;
};

inline std::deque<CLayer> cavern;

inline sf::VertexArray layer(float w, float h, sf::Color c) {
    sf::VertexArray l(sf::PrimitiveType::LineStrip);
    for (int i = 0; i <= l_detail; ++i) {
        float angle = (static_cast<float>(i) / l_detail) * 2.f * pi;

        float radX = w / 2.f;
        float radY = h / 2.f;

        float n = rand(-0.05f, 0.05f);
        float s = 0.f;

        if (angle > 0.2f * pi && angle < 0.8f * pi) {
            if (rand(0, 1) > 0.85f) s = -rand(h * 0.1f, h * 0.25f);
        }
        else if (angle > 1.2f * pi && angle < 1.8f * pi) {
            if (rand(0, 1) > 0.80f) s = -rand(h * 0.15f, h * 0.4f);
        }

        sf::Vector2f vPos(std::cos(angle) * radX * (1.0f + n), std::sin(angle) * (radY + s) * (1.0f + n));
        l.append(sf::Vertex({ vPos, c }));
    }
    return l;
}

inline void initCavern() {
    for (int i = 0; i < mLayers; ++i) {
        cavern.push_back({ layer(1600.f, 900.f, sf::Color::White), (float)i });
    }
}

inline std::map<int, CMilestone> caveMilestones = {
    {60, {"This cave is much longer than I thought.", 5.f}},
    {160, {"Ugh... how much longer?!", 4.f}},
    {200, {"I'm starting to believe nothing is here...", 5.f}},
    {220, {"Can't turn back now...", 4.f}},
    {235, {"Is that... light up ahead?", 5.f}}
};

inline void checkDist() {
    for (auto& [distance, milestone] : caveMilestones) {
        if (lWalked >= distance && !milestone.triggered) {
            textQueue.push({ milestone.text, milestone.duration });
            milestone.triggered = true;
        }
    }
}