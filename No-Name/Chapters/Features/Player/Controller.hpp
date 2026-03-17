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
inline bool torchFlashed = false;

inline void sEntitiesCh() {
    if (chData.empty() || enData.size() > 10) return;

    int index = std::rand() % chData.size();
    auto& spawnCh = chData[index];

    if (spawnCh.chPos == p.chunk) return;

    float r = static_cast<float>(std::rand() % 100);
    Entities t;
    if      (r > 95.f) t = Entities::SHINING;
    else if (r > 80.f) t = Entities::LURKER;
    else if (r > 65.f) t = Entities::WRAITH;
    else if (r > 50.f) t = Entities::STALKER;
    else if (r > 35.f) t = Entities::HOLLOW;
    else if (r > 20.f) t = Entities::HUSK;
    else               t = Entities::LEECH;

    float offsetX = static_cast<float>(std::rand() % static_cast<int>(chSize));
    float offsetY = static_cast<float>(std::rand() % static_cast<int>(chSize));
    sf::Vector2f basePos = { spawnCh.chPos.x * chSize + offsetX, spawnCh.chPos.y * chSize + offsetY };

    sEntity(t, basePos, spawnCh.chPos);

    if (t == Entities::HUSK) {
        int groupSize = 1 + (std::rand() % 2);
        for (int i = 0; i < groupSize; ++i) {
            float gx = static_cast<float>(std::rand() % static_cast<int>(chSize));
            float gy = static_cast<float>(std::rand() % static_cast<int>(chSize));
            sEntity(Entities::HUSK, { spawnCh.chPos.x * chSize + gx, spawnCh.chPos.y * chSize + gy }, spawnCh.chPos);
        }
    }
}

inline void uEntities(float dt, sf::Vector2f playerPos) {
    isParanoid = false;
    maxEntitySuspicion = 0.f;

    auto it = enData.begin();
    while (it != enData.end()) {
        if (it->type == Entities::LURKER) isParanoid = true;
        if (it->type == Entities::STALKER) isParanoid = true;

        sf::Vector2f prevPos = it->enPos;

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
            else if (it->type == Entities::WRAITH) {
                sanity -= 3;
            }
            else if (it->type == Entities::STALKER) {
                sanity -= 2;
                float angle = static_cast<float>(std::rand() % 360) * pi / 180.f;
                float tDist = chSize * (rDist - 0.5f);
                it->enPos = playerPos + sf::Vector2f(std::cos(angle) * tDist, std::sin(angle) * tDist);
                it->stalkerTimer = 5.f;
                ++it;
                continue;
            }
            else {
                if (shield > 0) consumeShield();
                else hp -= (it->damage + it->damageP);
                sanity -= (it->sDrain);
            }
            it->delEntity = true;
        }

        if (it->delEntity) {
            it = enData.erase(it);
            continue;
        }

        if (it->stunTimer > 0.f) {
            it->stunTimer -= dt;
            it->entity[0].color = sf::Color::Yellow;

            if (it->type == Entities::HUSK) {
                for (auto& other : enData) {
                    if (&other != &(*it) && other.type == Entities::HUSK && other.stunTimer <= 0.f) {
                        sf::Vector2f hDiff = other.enPos - it->enPos;
                        float hDist = std::sqrt(hDiff.x * hDiff.x + hDiff.y * hDiff.y);
                        if (hDist < chSize * 2.f) other.stunTimer = 3.f;
                    }
                }
            }
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

        float rad = chSize * 5.f;
        if (dist < rad) {
            it->suspicion += (rad - dist) / 100.f * dt;
        }
        else {
            it->suspicion -= 0.5f * dt;
        }
        it->suspicion = std::clamp(it->suspicion, 0.f, 5.f);
        maxEntitySuspicion = std::max(maxEntitySuspicion, it->suspicion);

        if (it->type == Entities::SHINING)
            it->orbitAngle += 0.45f * dt;

        if (it->type == Entities::WRAITH) {
            if (it->orbitRadius == 0.f) it->orbitRadius = chSize * 3.f;

            it->orbitAngle += 1.2f * dt;
            it->orbitRadius -= 8.f * dt;
            if (it->orbitRadius < 60.f) it->orbitRadius = 60.f;

            sf::Vector2f target(
                playerPos.x + std::cos(it->orbitAngle) * it->orbitRadius,
                playerPos.y + std::sin(it->orbitAngle) * it->orbitRadius
            );
            sf::Vector2f wDiff = target - it->enPos;
            float wDist = std::sqrt(wDiff.x * wDiff.x + wDiff.y * wDiff.y);
            if (wDist > 0.1f) it->enPos += (wDiff / wDist) * 120.f * dt;

            if (dist < chSize * 4.f) {
                sf::Vector2f vDir = (dist > 0.1f) ? diff / dist : sf::Vector2f(0.f, 1.f);
                float dot = (p.facing.x * (-vDir.x)) + (p.facing.y * (-vDir.y));

                if (torchOn && dot > 0.6f) {
                    it->orbitRadius += 200.f * dt;
                    it->stunTimer = 1.5f;
                }

                if (torchFlashed && dist < chSize * 1.5f && dot > 0.6f) {
                    it->delEntity = true;
                }
            }

            if (dist < 80.f) {
                it->sanityDrainTimer += dt;
                if (it->sanityDrainTimer >= 5.f) {
                    sanity -= 1;
                    it->sanityDrainTimer = 0.f;
                }
            } else {
                it->sanityDrainTimer = 0.f;
            }
        }
        else if (it->type == Entities::HOLLOW) {
            if (isWalking) {
                it->suspicion += (rad - dist) / 80.f * dt;
            } else {
                it->suspicion -= 1.5f * dt;
            }
            it->suspicion = std::clamp(it->suspicion, 0.f, 5.f);

            if (it->suspicion >= 1.5f) {
                it->enPos += (-dir) * 60.f * dt;
            } else {
                uPatrol(*it, dt);
            }
        }
        else if (it->type == Entities::STALKER) {
            it->stalkerTimer -= dt;

            bool playerLooking = (lookFac > 0.85f && dist < chSize * (rDist + 1.f));
            if (playerLooking) {
                it->stalkerTimer = 5.f;
                float angle = static_cast<float>(std::rand() % 360) * pi / 180.f;
                float tDist = chSize * (rDist - 0.5f);
                it->enPos = playerPos + sf::Vector2f(std::cos(angle) * tDist, std::sin(angle) * tDist);
            }

            if (it->stalkerTimer <= 0.f) {
                it->enPos += (-dir) * 15.f * dt;
                if (dist < chSize * (rDist - 2.f)) {
                    float angle = static_cast<float>(std::rand() % 360) * pi / 180.f;
                    float tDist = chSize * (rDist - 0.5f);
                    it->enPos = playerPos + sf::Vector2f(std::cos(angle) * tDist, std::sin(angle) * tDist);
                    it->stalkerTimer = 4.f;
                }
            }

            it->orbitAngle += 0.8f * dt;
            float pulse = (std::sin(it->orbitAngle) + 1.f) / 2.f;
            uint8_t baseA  = 30;
            uint8_t rangeA = 35;
            uint8_t pAlpha = static_cast<uint8_t>(baseA + pulse * rangeA);

            size_t vCount = it->entity.getVertexCount();
            size_t eyeStart = vCount >= 4 ? vCount - 4 : 0;
            for (size_t vi = 0; vi < vCount; ++vi) {
                uint8_t a = (vi >= eyeStart) ? std::min(255, static_cast<int>(pAlpha) + 40) : pAlpha;
                it->entity[vi].color.a = a;
            }

            it->sanityDrainTimer += dt;
            if (it->sanityDrainTimer >= 20.f) {
                sanity -= 1;
                it->sanityDrainTimer = 0.f;
            }
        }
        else if (it->type == Entities::HUSK) {
            float hSpeed = 110.f;
            sf::Vector2f nextPos = it->enPos + it->husklDir * hSpeed * dt;

            if (nextPos.x < chBounds.minX || nextPos.x > chBounds.maxX) {
                it->husklDir.x = -it->husklDir.x;
                nextPos = it->enPos + it->husklDir * hSpeed * dt;
            }
            if (nextPos.y < chBounds.minY || nextPos.y > chBounds.maxY) {
                it->husklDir.y = -it->husklDir.y;
                nextPos = it->enPos + it->husklDir * hSpeed * dt;
            }
            it->enPos = nextPos;
        }
        else if (it->suspicion < 1.5f) {
            uPatrol(*it, dt);
        }
        else {
            float speed = (it->type == Entities::LEECH) ? 150.f : 80.f;
            if (it->type == Entities::SHINING) speed = 40.f;
            it->enPos += (-dir) * speed * dt;
        }

        if (it->type != Entities::STALKER &&
            it->type != Entities::WRAITH  &&
            it->type != Entities::SHINING) {
            sf::Vector2f moved = it->enPos - prevPos;
            if (std::abs(moved.x) > 0.5f * dt)
                it->facingRight = (moved.x > 0.f);
            float moveDist = std::sqrt(moved.x * moved.x + moved.y * moved.y);
            it->animTimer += moveDist * 0.05f;
            if (it->animTimer >= 1.f) {
                it->animTimer -= 1.f;
                it->animStep ^= 1;
            }
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
        sanity = std::min(10.f, sanity + 3);
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
        if (c.chPos == p.chunk) {
            if (c.type == Chunks::FOREST)  { maxSpd *= 0.6f; break; }
            if (c.type == Chunks::MARSH)   { maxSpd *= 0.4f; break; }
            if (c.type == Chunks::BRAMBLE) { maxSpd *= 0.5f; break; }
        }
    }

    bool shiftHeld = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift);
    bool isMoving  = (velocity.x != 0.f || velocity.y != 0.f);
    isSprinting = shiftHeld && stamina > 0.f && isMoving;

    if (isSprinting) {
        maxSpd *= 1.3f;
        stamina -= staminaDrain * dt;
        if (stamina < 0.f) stamina = 0.f;
    } else {
        stamina += staminaRegen * dt;
        if (stamina > 100.f) stamina = 100.f;
    }

    if (isMoving) {
        float l = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        velocity /= l;
        p.pos += velocity * maxSpd * dt;
        p.facing = velocity;
        isWalking = true;
    } else {
        isWalking = false;
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

    static bool qPressed = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
        if (!qPressed) {
            dropActiveItem(p.pos);
            qPressed = true;
        }
    }
    else qPressed = false;

    static bool fPressed = false;
    torchFlashed = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) {
        if (!fPressed) {
            torchOn = !torchOn;
            if (torchOn) torchFlashed = true;
            fPressed = true;
        }
    }
    else fPressed = false;

    if (torchOn) {
        float drainRate = isSprinting ? 3.0f : 2.0f;
        battery -= drainRate * dt;
        if (battery <= 0) { battery = 0; torchOn = false; }
    }

    shield = 0;
    if (!isUltra) {
        for (auto& item : inventory) if (item.type == ItemType::SHIELD) { shield = 1; break; }
    }
}

inline void initPlayer(sf::RenderWindow& window) {
    sf::Vector2f centre(window.getSize().x / 2.f, window.getSize().y / 2.f);

    const float vRadBase   = chSize * (rDist + 0.5f);
    const float glowRadBase = chSize * 1.2f;

    float sanityFac = 1.f - (hallucinations * 0.65f);
    const float vRad   = vRadBase   * sanityFac;
    const float glowRad = glowRadBase * sanityFac;
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

        bool shouldFlip = (en.type == Entities::LURKER ||
                           en.type == Entities::HOLLOW ||
                           en.type == Entities::HUSK   ||
                           en.type == Entities::LEECH);
        if (shouldFlip && !en.facingRight)
            tx.scale({ -1.f, 1.f });

        if (shouldFlip) {
            float stepY = (en.animStep == 1) ? 2.f : 0.f;
            float stepX = (en.animStep == 1) ? 1.f : -1.f;
            tx.translate({ stepX, stepY });
        }

        uint8_t aVal = alpha(en.enPos);

        for (size_t i = 0; i < en.entity.getVertexCount(); ++i) {
            sf::Color col;
            if (en.type == Entities::LEECH && en.entity[i].color == sf::Color::Red) {
                col = sf::Color::Red;
                col.a = aVal;
            }
            else if (en.type == Entities::STALKER) {
                uint8_t baseA = en.entity[i].color.a;
                col = sf::Color::White;
                col.a = static_cast<uint8_t>(std::min(static_cast<int>(aVal), static_cast<int>(baseA)));
            }
            else {
                col = sf::Color::White;
                col.a = aVal;
            }
            en.entity[i].color = col;
        }

        if (en.type == Entities::SHINING) {
            tx.rotate(sf::degrees(en.orbitAngle * (180.f / pi)));
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
                    va.append({ {std::cos(a) * 10, std::sin(a) * 10}, trapCol });
                    va.append({ {std::cos(a) * 15, std::sin(a) * 15}, trapCol });
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

    if (maxEntitySuspicion > 0.2f) {
        bool chasing = (maxEntitySuspicion >= 1.5f);
        float t = chasing
            ? std::clamp((maxEntitySuspicion - 1.5f) / 3.5f, 0.f, 1.f)
            : std::clamp((maxEntitySuspicion - 0.2f) / 1.3f, 0.f, 1.f);

        uint8_t r = chasing ? static_cast<uint8_t>(110.f + t * 60.f) : 220;
        uint8_t g = chasing ? static_cast<uint8_t>(8.f)               : 220;
        uint8_t b = chasing ? static_cast<uint8_t>(8.f)               : 220;
        uint8_t a = chasing ? static_cast<uint8_t>(30.f + t * 55.f)  : static_cast<uint8_t>(t * 28.f);

        sf::Color edge(r, g, b, a);
        sf::Color fade(r, g, b, 0);

        float W  = static_cast<float>(window.getSize().x);
        float H  = static_cast<float>(window.getSize().y);
        float vW = W * 0.07f;
        float vH = H * 0.09f;

        auto drawStrip = [&](sf::Vector2f a0, sf::Vector2f a1,
                             sf::Vector2f b0, sf::Vector2f b1,
                             sf::Color cA, sf::Color cB) {
            sf::VertexArray strip(sf::PrimitiveType::TriangleStrip, 4);
            strip[0] = { a0, cA }; strip[1] = { b0, cB };
            strip[2] = { a1, cA }; strip[3] = { b1, cB };
            window.draw(strip);
        };

        drawStrip({0,0},   {0,H},   {vW,0},    {vW,H},    edge, fade);
        drawStrip({W,0},   {W,H},   {W-vW,0},  {W-vW,H},  edge, fade);
        drawStrip({0,0},   {W,0},   {0,vH},    {W,vH},    edge, fade);
        drawStrip({0,H},   {W,H},   {0,H-vH},  {W,H-vH},  edge, fade);
    }
}