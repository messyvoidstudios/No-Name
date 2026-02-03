#pragma once
#include "../../../Misc/Includes.hpp"
#include "../../../Misc/Variables.hpp"

#include "../../Features/Blink.hpp"

#include "../../../UI/Text.hpp"

inline sf::VertexArray smiles(float size, sf::Color color) {
    sf::VertexArray smile(sf::PrimitiveType::Lines);
    int segments = 16;
    for (int i = 0; i < segments; ++i) {
        float angle1 = (static_cast<float>(i) / segments) * pi;
        float angle2 = (static_cast<float>(i + 1) / segments) * pi;

        sf::Vector2f p1(std::cos(angle1) * size, std::sin(angle1) * (size * 0.5f));
        sf::Vector2f p2(std::cos(angle2) * size, std::sin(angle2) * (size * 0.5f));

        smile.append(sf::Vertex({ p1, color }));
        smile.append(sf::Vertex({ p2, color }));
    }
    return smile;
}

inline void uShining(Blinking& blink, float dt, sf::RenderWindow& window, sf::Vector2f bobOffset) {
    static bool shiningTriggered = false;
    static bool caveRegenerated = false;

    sf::Vector2f centre(window.getSize().x / 2.f, window.getSize().y / 2.f);

    if (lWalked >= 235 && !shiningTriggered && !deadFromShining) {
        shining = true;
        shiningTriggered = true;
        shiningGrace = 15.f;
        shiningDepth = 10.f;

        if (!caveRegenerated) {
            l_detail = 240;
            cavern.clear();
            for (int i = 0; i < mLayers; ++i) {
                cavern.push_back({ layer(1600.f, 900.f, sf::Color::White), (float)i });
            }
            caveRegenerated = true;
        }
    }

    if (!shining) return;

    if (lWalked >= 240) {
        shiningDepth -= 1.0f * dt;
        shiningGrace -= dt;

        static bool messagePushed = false;
        if (!messagePushed) {
            textQueue.push({ "The light is approaching... CLOSE YOUR EYES!!!", 3.f });
            messagePushed = true;
        }
    }

    if (blink.progress >= 1.0f) {
        shining = false;
        textQueue.push({ "It's gone... What was that...?", 3.f });
        return;
    }

    if (shiningDepth <= 0.2f) {
        shining = false;
        deadFromShining = true;
        return;
    }

    float t = std::clamp((10.f - shiningDepth) / 10.f, 0.0f, 1.0f);
    uint8_t a = static_cast<uint8_t>(20.f + (t * (255.f - 20.f)));
    float pulse = std::abs(std::sin(shiningGrace * 8.f));
    uint8_t intensity = static_cast<uint8_t>(200 + (55 * pulse));
    sf::Color color(255, 255, intensity, a);

    std::vector<sf::Vector2f> smileOffsets = {
        {0, 0}, {-150, -100}, {180, -80}, {-120, 150}, {140, 120}
    };

    float scale = std::pow(0.75f, std::max(0.1f, shiningDepth));
    float jIntensity = (1.0f - t) * 10.f + (t * 40.f);

    sf::Transform tx;
    tx.translate(centre + bobOffset);
    tx.scale({ scale, scale });
    tx.translate({ std::sin(shiningGrace * 15.f) * jIntensity, 0.f });

    sf::VertexArray entity = smiles(40.f, color);

    for (size_t i = 0; i < smileOffsets.size(); ++i) {
        sf::Transform ltx = tx;

        ltx.translate(smileOffsets[i]);

        float rotation = std::sin(shiningGrace * 2.f + i) * 20.f + (i * 30.f);
        ltx.rotate(sf::degrees(rotation));

        window.draw(entity, ltx);
    }
}