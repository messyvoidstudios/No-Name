#pragma once
#include "../../../Misc/Functions.hpp"
#include "../../../Misc/Includes.hpp"
#include "../../../Misc/Variables.hpp"

#include "../Effects/Dust.hpp"

#include "../../../UI/Text.hpp"

struct CLayer {
    sf::VertexArray cave;
    float depth;
};

struct CMilestone {
    std::string text;
    float duration;
    bool triggered = false;
};

inline std::map<int, CMilestone> caveMilestones = {
    {60, {"This cave is much longer than I thought.", 3.f}},
    {160, {"Ugh... how much longer?!", 3.f}},
    {200, {"I'm starting to believe nothing is here...", 3.f}},
    {220, {"Can't turn back now...", 3.f}},
    {235, {"What happened to the cave...?", 3.f}},
    {237, {"Is that... light up ahead?", 3.f}}
};

inline void checkDist() {
    for (auto& [distance, milestone] : caveMilestones) {
        if (lWalked >= distance && !milestone.triggered) {
            textQueue.push({ milestone.text, milestone.duration });
            milestone.triggered = true;
        }
    }
}

inline std::deque<CLayer> cavern;

inline sf::VertexArray layer(float w, float h, sf::Color c) {
    sf::VertexArray l(sf::PrimitiveType::LineStrip);
    sf::Vector2f sPos;
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

        if (i == 0) sPos = vPos;

        if (i == l_detail) l.append(sf::Vertex({ sPos, c }));
        else l.append(sf::Vertex({ vPos, c }));
    }
    return l;
}

inline void uCavern(float et, sf::Vector2f bobOffset, sf::RenderWindow& window) {
    sf::Vector2f centre(window.getSize().x / 2.f, window.getSize().y / 2.f);
    for (int i = (int)cavern.size() - 1; i >= 0; --i) {
        auto& layer = cavern[i];

        float scale = std::pow(0.75f, std::max(0.01f, layer.depth));
        float lFac = std::pow(0.7f, std::max(0.01f, layer.depth));
        float pulse = (std::sin(et * 0.5f) + 1.0f) / 2.0f;

        float aFac = 1.0f;
        if (layer.depth < 1.5f) {
            aFac = std::clamp((layer.depth - 0.2f) / 1.3f, 0.0f, 1.0f);
        }
        else if (layer.depth > (mLayers - 3)) {
            aFac = std::clamp((mLayers - layer.depth) / 3.0f, 0.0f, 1.0f);
        }

        uint8_t col = static_cast<uint8_t>(255 * lFac * (0.5f + pulse * 0.5f));
        uint8_t a = static_cast<uint8_t>(255 * aFac);

        sf::Color c;
        if (shining) c = sf::Color(col, col * 0.2f, col * 0.2f, a);
        else c = sf::Color(col, col, col, a);

        for (size_t v = 0; v < layer.cave.getVertexCount(); ++v) {
            layer.cave[v].color = c;
        }

        sf::Transform tx;
        tx.translate(centre + bobOffset);
        tx.scale({ scale, scale });
        float drift = std::max(0.f, layer.depth);
        tx.translate({ drift * 10.f, drift * 5.f });

        window.draw(layer.cave, tx);
    }

    if (lWalked == 234) {
        blind(window);
    }
}

inline void uCWalk(float& bobbing, sf::Vector2f& bobOffset) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) isWalking = true;
    else isWalking = false;

    if (isWalking) {
        for (auto& layer : cavern) layer.depth -= walkSpd;
        bobbing += 0.12f;
        bobOffset.x = std::cos(bobbing * 0.5f) * 12.f;
        bobOffset.y = std::sin(bobbing) * 8.f;

        if (cavern.front().depth <= -1.0f) {
            cavern.pop_front();
            cavern.push_back({ layer(1600.f, 900.f, sf::Color::White), cavern.back().depth + 1.0f });
            lWalked++;
        }
    }

    for (auto& d : dust) {
        float curSpd = isWalking ? (walkSpd * d.spd) : dustDrift;
        d.depth -= curSpd;

        if (d.depth < 0) {
            d.depth = 10.f;
            d.pos = { rand(-1000, 1000), rand(-600, 600) };
        }
    }
}

inline void initCavern() {
    for (int i = 0; i < mLayers; ++i) {
        cavern.push_back({ layer(1600.f, 900.f, sf::Color::White), (float)i });
    }
}