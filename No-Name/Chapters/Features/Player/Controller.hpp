#pragma once
#include "../../../Misc/Functions.hpp"
#include "../../../Misc/Includes.hpp"
#include "../../../Misc/Variables.hpp"

#include "../Blink.hpp"
#include "../Inventory.hpp"
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
    if (chData.empty() || enData.size() > 12) return;

    int index = std::rand() % chData.size();
    auto& spawnCh = chData[index];

    if (spawnCh.chPos == p.chunk) return;

    float r = static_cast<float>(std::rand() % 100);
    Entities t = (r > 90.f) ? Entities::SHINING : (r > 65.f) ? Entities::LURKER : Entities::LEECH;

    float offsetX = static_cast<float>(std::rand() % (int)chSize);
    float offsetY = static_cast<float>(std::rand() % (int)chSize);

    sEntity(t, { spawnCh.chPos.x * chSize + offsetX, spawnCh.chPos.y * chSize + offsetY }, spawnCh.chPos);
}

inline void uEntities(float dt, sf::Vector2f playerPos) {
    isParanoid = false;

    auto it = enData.begin();
    while (it != enData.end()) {
        if (it->type == Entities::LURKER) isParanoid = true;

        it->enChunk = sf::Vector2i(
            static_cast<int>(std::floor(it->enPos.x / chSize)),
            static_cast<int>(std::floor(it->enPos.y / chSize))
        );

        sf::Vector2f diff = it->enPos - playerPos;
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        sf::Vector2f dir = (dist > 0.1f) ? diff / dist : sf::Vector2f(0.f, -1.f);

        float lookFac = (p.facing.x * dir.x) + (p.facing.y * dir.y);

        if (it->type == Entities::SHINING && blink.progress >= 1.f) it->delEntity = true;

        if (it->type == Entities::LURKER && lookFac > 0.7f && dist < chSize * 3.f) it->delEntity = true;

        if (it->isHallucination && dist < 25.f) it->delEntity = true;

        if (dist > chSize * (rDist + 1.5f)) it->delEntity = true;

        if (dist < 15.f && !it->isHallucination) {
            if (it->type == Entities::LEECH) {
                stealItem();
            }
            else {
                if (shield > 0) consumeShield();
                else hp -= (it->damage + it->damageP);
                sanity -= (it->sDrain);
            }
            it->delEntity = true;
        }

        if (it->stunTimer > 0.f) {
            it->stunTimer -= dt;
            it->entity[0].color = sf::Color::Yellow;
            ++it;
            continue;
        }

        for (auto& trap : activeTraps) {
            if (!trap.sprung) {
                sf::Vector2f tDiff = it->enPos - trap.pos;
                float tDist = std::sqrt(tDiff.x * tDiff.x + tDiff.y * tDiff.y);

                if (tDist < 25.f) {
                    trap.sprung = true;
                    it->stunTimer = 6.f;
                }
            }
        }

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
            it->enPos += (-dir) * speed * dt;
        }

        ++it;
    }
}

inline void useItem() {
    if (activeSlot >= inventory.size()) return;
    auto& item = inventory[activeSlot];

    bool consumed = false;
    if (item.type == ItemType::BATTERY) {
        battery = 100.f;
        consumed = true;
    }
    else if (item.type == ItemType::PILLS) {
        sanity = std::min(10, sanity + 3);
        consumed = true;
    }
    else if (item.type == ItemType::TRAP) {
        activeTraps.push_back({ p.pos, false, 60.f });
        consumed = true;
    }

    if (consumed) {
        item.count--;
        if (item.count <= 0) inventory.erase(inventory.begin() + activeSlot);
    }
}

inline void uPlayer(float dt) {
    sf::Vector2f velocity(0.f, 0.f);
    p.chunk = sf::Vector2i(static_cast<int>(std::floor(p.pos.x / chSize)), static_cast<int>(std::floor(p.pos.y / chSize)));

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) velocity.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) velocity.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) velocity.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) velocity.x += 1.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) activeSlot = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) activeSlot = 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)) activeSlot = 2;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4)) activeSlot = 3;

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

    static bool lClick = false;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (!lClick) {
            useItem();
            lClick = true;
        }
    }
    else lClick = false;

    static bool fPressed = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) {
        if (!fPressed) {
            torchOn = !torchOn;
            fPressed = true;
        }
    }
    else fPressed = false;

    if (torchOn) {
        battery -= 2.0f * dt;
        if (battery <= 0) { battery = 0; torchOn = false; }
    }

    shield = 0;
    if (!isBrutal) {
        for (auto& item : inventory) if (item.type == ItemType::SHIELD) { shield = 1; break; }
    }
}

inline void initPlayer(sf::RenderWindow& window) {
    sf::Vector2f centre(window.getSize().x / 2.f, window.getSize().y / 2.f);

    const float vRad = chSize * (rDist + 0.5f);
    const float glowRad = chSize * 1.2f;
    const float torchL = chSize * 7.f;
    const float torchW = 0.7f;

    auto alpha = [&](sf::Vector2f wPos) {
        sf::Vector2f diff = wPos - p.pos;
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        if (dist < 5.f) return (uint8_t)255;

        float ambGlow = 1.f - std::clamp((dist - glowRad) / (vRad - glowRad), 0.f, 1.f);
        ambGlow = std::pow(ambGlow, 3.f);

        float torch = 0.f;
        if (torchOn && dist < torchL) {
            sf::Vector2f vDir = diff / dist;
            float dot = (p.facing.x * vDir.x) + (p.facing.y * vDir.y);
            if (dot > torchW) {
                torch = ((dot - torchW) / (1.f - torchW)) * (1.f - (dist / torchL));
            }
        }
        return static_cast<uint8_t>(255 * std::clamp(ambGlow + torch, 0.f, 1.f));
    };

    for (auto& c : chData) {
        sf::Transform tx;
        tx.translate(centre);
        sf::Vector2f wPos(c.chPos.x * chSize, c.chPos.y * chSize);
        tx.translate(wPos - p.pos);

        for (size_t i = 0; i < c.chunk.getVertexCount(); ++i) {
            c.chunk[i].color.a = alpha(wPos + c.chunk[i].position);
        }
        window.draw(c.chunk, tx);

        if (!c.showEyes && c.eyes.getVertexCount() > 0) {
            for (size_t i = 0; i < c.eyes.getVertexCount(); ++i)
                c.eyes[i].color.a = alpha(wPos + c.eyes[i].position);
            window.draw(c.eyes, tx);
        }
    }

    for (auto& en : enData) {
        sf::Transform tx;
        tx.translate(centre);
        tx.translate(en.enPos - p.pos);

        uint8_t aVal = alpha(en.enPos);

        for (size_t i = 0; i < en.entity.getVertexCount(); ++i) {
            sf::Color col = (en.type == Entities::LEECH && en.entity[i].color == sf::Color::Red)
                ? sf::Color::Red : sf::Color::White;
            col.a = aVal;
            en.entity[i].color = col;
        }

        if (en.type == Entities::SHINING) {
            float angle = std::sin(lWalked * 0.1f) * 15.f;
            tx.rotate(sf::degrees(angle));
        }

        if (aVal > 2) window.draw(en.entity, tx);
    }

    for (auto& wi : worldItems) {
        uint8_t aVal = alpha(wi.pos);
        if (aVal > 10) {
            sf::Transform tx;
            tx.translate(centre);
            tx.translate(wi.pos - p.pos);

            sf::Color itemCol = (wi.type == ItemType::BATTERY) ? sf::Color(60, 60, 60) :
                (wi.type == ItemType::SHIELD) ? sf::Color(40, 40, 40) :
                (wi.type == ItemType::PILLS) ? sf::Color(60, 60, 60) : sf::Color(40, 40, 40);
            itemCol.a = aVal;

            sf::VertexArray shape = getItemShape(wi.type, { 0, 0 }, itemCol, 1.0f);
            window.draw(shape, tx);
        }
    }

    for (auto& trap : activeTraps) {
        uint8_t aVal = alpha(trap.pos);
        if (aVal > 10) {
            sf::Transform tx;
            tx.translate(centre + (trap.pos - p.pos));

            sf::Color trapCol = trap.sprung ? sf::Color(100, 100, 100, aVal) : sf::Color(200, 200, 200, aVal);

            sf::VertexArray va(sf::PrimitiveType::Lines);
            if (!trap.sprung) {
                for (int i = 0; i < 6; ++i) {
                    float a = (i / 6.f) * 6.28f;
                    va.append({ {cos(a) * 10, sin(a) * 10}, trapCol });
                    va.append({ {cos(a) * 15, sin(a) * 15}, trapCol });
                }
            }
            else {
                va.append({ {-10, -10}, trapCol }); va.append({ {10, 10}, trapCol });
                va.append({ {10, -10}, trapCol }); va.append({ {-10, 10}, trapCol });
            }
            window.draw(va, tx);
        }
    }

    sf::RectangleShape pDot({ 8.f, 8.f });
    pDot.setOrigin({ 4.f, 4.f });
    pDot.setPosition(centre);
    pDot.setFillColor(sf::Color::White);
    window.draw(pDot);
}