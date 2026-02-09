#pragma once
#include "../../Misc/Functions.hpp"
#include "../../Misc/Includes.hpp"
#include "../../Misc/Variables.hpp"

#include "../../UI/Text.hpp"

struct Fracture {
    sf::VertexArray lines;
    bool isCracking = false;
    float a = 0.f;
    float timer = 0.f;
};

inline Fracture crack;

inline void cracks(sf::VertexArray& crack, sf::Vector2f start, sf::Vector2f end, int depth) {
    if (depth <= 0) return;

    sf::Vector2f mid = (start + end) / 2.f;
    float offset = rand(-20.f, 20.f);
    mid.x += offset;
    mid.y += offset;

    crack.append(sf::Vertex({ start, sf::Color::White }));
    crack.append(sf::Vertex({ mid, sf::Color::White }));
    crack.append(sf::Vertex({ mid, sf::Color::White }));
    crack.append(sf::Vertex({ end, sf::Color::White }));

    if (rand(0, 1) > 0.8f) {
        sf::Vector2f branchEnd = mid + sf::Vector2f(rand(-100, 100), rand(-100, 100));
        cracks(crack, mid, branchEnd, depth - 1);
    }

    cracks(crack, start, mid, depth - 1);
    cracks(crack, mid, end, depth - 1);
}

inline void uFracture(Blinking& blink, float dt, sf::RenderWindow& window) {
    sf::Vector2f centre(window.getSize().x / 2.f, window.getSize().y / 2.f);
    if (lWalked >= 260) {
        if (blink.progress >= 1.f && !crack.isCracking) {
            crack.isCracking = true;
            crack.lines.setPrimitiveType(sf::PrimitiveType::Lines);

            for (int i = 0; i < 10; ++i) {
                float angle = (i / 10.f) * 2.f * pi;
                sf::Vector2f edge(centre.x + std::cos(angle) * 1200.f, centre.y + std::sin(angle) * 1200.f);
                cracks(crack.lines, centre, edge, 4);
            }

            static bool tPushed = false;
            if (!tPushed) {
                textQueue.push({ "What the-", 3.f });
                tPushed = true;
            }
        }
    }

    if (crack.isCracking) {
        if (crack.a < 255.f) crack.a += 150.f * dt;
        for (size_t i = 0; i < crack.lines.getVertexCount(); ++i) {
            crack.lines[i].color.a = static_cast<uint8_t>(crack.a);
        }

        blind(window);
        window.draw(crack.lines);
    }
}