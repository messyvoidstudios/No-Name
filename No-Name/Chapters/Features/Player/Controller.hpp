#pragma once
#include "../../../Misc/Functions.hpp"
#include "../../../Misc/Includes.hpp"
#include "../../../Misc/Variables.hpp"

#include "../Blink.hpp"

#include "../../Chapter-1/Chunks.hpp"
#include "../../Chapter-1/Entities.hpp"

struct Player {
    float spd = chSize * 2.f;
    sf::Vector2f pos = { 0.f, 0.f };
    sf::Vector2i chunk = { 0, 0 };
    sf::Vector2f facing = { 0.f, -1.f };
};

inline Player p;

inline void sEntitiesCh() {
    if (chData.empty() || enData.size() > 15) return;

    int index = std::rand() % chData.size();
    auto& spawnCh = chData[index];

    if (spawnCh.chPos == p.chunk) return;

    float r = rand(0.f, 100.f);
    Entities t = (r > 90.f) ? Entities::SHINING : (r > 60.f) ? Entities::LURKER : Entities::LEECH;

    sEntity(t, { spawnCh.chPos.x * chSize + 100.f, spawnCh.chPos.y * chSize + 100.f }, spawnCh.chPos);
}

inline void uEntities(sf::Vector2f playerPos, float dt) {
    auto it = enData.begin();
    while (it != enData.end()) {
        it->enChunk = sf::Vector2i(
            static_cast<int>(std::floor(it->enPos.x / chSize)),
            static_cast<int>(std::floor(it->enPos.y / chSize))
        );

        sf::Vector2f diff = it->enPos - playerPos;
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        sf::Vector2f dir = diff / dist;

        float lookFac = (p.facing.x * dir.x) + (p.facing.y * dir.y);

        if (it->type == Entities::SHINING && blink.progress >= 1.f) {
            it->delEntity = true;
        }

        if (it->type == Entities::LURKER && lookFac > 0.7f && dist < chSize * 3.f) {
            it->delEntity = true;
        }

        if (it->type == Entities::LEECH && dist < 15.f) {
            it->delEntity = true;
        }

        if (dist > chSize * (rDist + 1.2f)) it->delEntity = true;

        if (it->delEntity) {
            it = enData.erase(it);
            continue;
        }

        float rad = chSize * 5.f;
        if (dist < rad) {
            it->suspicion += (rad - dist) / 100.f * dt;
        }
        else {
            it->suspicion -= 0.5f * dt;
        }
        it->suspicion = std::clamp(it->suspicion, 0.f, 5.f);

        if (it->suspicion < 1.5f) {
            uPatrol(*it, dt);
        }
        else {
            float speed = (it->type == Entities::LEECH) ? 150.f : 80.f;
            if (it->type == Entities::SHINING) speed = 40.f;

            sf::Vector2f mDir = -dir;
            it->enPos += mDir * speed * dt;
        }

        ++it;
    }
}

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
        p.facing = velocity;
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

    for (auto& en : enData) {
        sf::Transform tx;
        tx.translate(centre);
        tx.translate(en.enPos - p.pos);

        float dx = p.pos.x - en.enPos.x;
        float dy = p.pos.y - en.enPos.y;
        float dist = std::sqrt(dx * dx + dy * dy);

        float a = 1.0f - std::clamp((dist - rad) / (vRad - rad), 0.0f, 1.0f);
        a = std::pow(a, 2.0f);
        uint8_t alphaValue = static_cast<uint8_t>(255 * a);

        for (size_t i = 0; i < en.entity.getVertexCount(); ++i) {
            if (en.type == Entities::LEECH && en.entity[i].color == sf::Color::Red) {
                en.entity[i].color.a = alphaValue;
            }
            else {
                en.entity[i].color = sf::Color(255, 255, 255, alphaValue);
            }
        }

        if (en.type == Entities::SHINING) {
            float angle = std::sin(chWalked * 0.1f) * 15.f;
            tx.rotate(sf::degrees(angle));
        }

        if (alphaValue > 0) {
            window.draw(en.entity, tx);
        }
    }

    sf::RectangleShape pDot({ 8.f, 8.f });
    pDot.setOrigin({ 4.f, 4.f });
    pDot.setPosition(centre);
    pDot.setFillColor(sf::Color::White);
    window.draw(pDot);
}