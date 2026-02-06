#pragma once
#include "../../../Misc/Functions.hpp"
#include "../../../Misc/Includes.hpp"
#include "../../../Misc/Variables.hpp"

#include "../../Chapter-1/V-2X-A/V-2X-A.hpp"

struct Player {
    sf::Vector2f pos = { 0.f, 0.f };
    float spd = chSize * 2.f;
    sf::Vector2i chunk = { 0, 0 };
};

inline Player p;

inline void uPlayer(float dt) {
    sf::Vector2f velocity(0.f, 0.f);

    p.chunk = sf::Vector2i(static_cast<int>(std::floor(p.pos.x / chSize)), static_cast<int>(std::floor(p.pos.y / chSize)));

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) velocity.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) velocity.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) velocity.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) velocity.x += 1.f;

    float maxSpd = p.spd;
    for (const auto& c : chData) {
        if (c.chPos == p.chunk && c.type == Chunks::FOREST) {
            maxSpd *= 0.6f;
            break;
        }
    }

    if (velocity.x != 0.f || velocity.y != 0.f) {
        float l = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        velocity /= l;
        p.pos += velocity * maxSpd * dt;
    }

    if (p.pos.x < chBounds.minX || p.pos.x > chBounds.maxX || p.pos.y < chBounds.minY || p.pos.y > chBounds.maxY) {
        rChunks(p.chunk);
    }
}

inline void init2X(sf::RenderWindow& window) {
    sf::Vector2f centre(window.getSize().x / 2.f, window.getSize().y / 2.f);

    const float vRad = chSize * (rDist + 0.2f);
    const float rad = chSize * 6.f;

    for (auto& c : chData) {
        sf::Transform tx;
        tx.translate(centre);
        sf::Vector2f wPos(c.chPos.x * chSize, c.chPos.y * chSize);
        tx.translate(wPos - p.pos);

        for (size_t i = 0; i < c.chunk.getVertexCount(); ++i) {
            sf::Vector2f vWPos = wPos + c.chunk[i].position;

            float dx = p.pos.x - vWPos.x;
            float dy = p.pos.y - vWPos.y;
            float dist = std::sqrt(dx * dx + dy * dy);

            float a = 1.0f - std::clamp((dist - rad) / (vRad - rad), 0.0f, 1.0f);

            a = std::pow(a, 2.0f);
            c.chunk[i].color.a = static_cast<uint8_t>(255 * a);
        }

        window.draw(c.chunk, tx);

        if (!c.showEyes && c.eyes.getVertexCount() > 0) {
            window.draw(c.eyes, tx);
        }
    }

    sf::RectangleShape pDot({ 8.f, 8.f });
    pDot.setOrigin({ 4.f, 4.f });
    pDot.setPosition(centre);
    pDot.setFillColor(sf::Color::White);
    window.draw(pDot);
}