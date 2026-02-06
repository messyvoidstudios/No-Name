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

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) velocity.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) velocity.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) velocity.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) velocity.x += 1.f;

    if (velocity.x != 0.f || velocity.y != 0.f) {
        float l = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        velocity /= l;
        p.pos += velocity * p.spd * dt;
    }

    if (p.pos.x < cBounds.minX || p.pos.x > cBounds.maxX || p.pos.y < cBounds.minY || p.pos.y > cBounds.maxY) {
        sf::Vector2i nChunk(static_cast<int>(std::floor(p.pos.x / chSize)), static_cast<int>(std::floor(p.pos.y / chSize)));
        rChunks(nChunk);
    }
}

inline void init2X(sf::RenderWindow& window) {
    sf::Vector2f centre(window.getSize().x / 2.f, window.getSize().y / 2.f);

    for (auto& c : activeChunks) {
        sf::Transform tx;
        tx.translate(centre);
        sf::Vector2f wPos(c.chPos.x * chSize, c.chPos.y * chSize);
        tx.translate(wPos - p.pos);

        window.draw(c.chunk, tx);
    }

    sf::RectangleShape pDot({ 10.f, 10.f });
    pDot.setOrigin({ 5.f, 5.f });
    pDot.setPosition(centre);
    pDot.setFillColor(sf::Color::White);
    window.draw(pDot);
}