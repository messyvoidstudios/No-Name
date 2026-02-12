#pragma once
#include "../../../Misc/Includes.hpp"
#include "../../../Misc/Variables.hpp"
#include "../Entities.hpp"

inline void uHallucinations(float dt, sf::Vector2f playerPos) {
    if (hallucinations < 0.2f) return;

    halluTimer += dt;

    float spawnThreshold = 15.f - (hallucinations * 10.f);

    if (halluTimer >= spawnThreshold) {
        halluTimer = 0.f;

        float angle = static_cast<float>(rand() % 360) * pi / 180.f;
        float dist = chSize * 5.f;
        sf::Vector2f spawnPos = playerPos + sf::Vector2f(std::cos(angle) * dist, std::sin(angle) * dist);

        EntityData fake;
        fake.type = (rand() % 2 == 0) ? Entities::LURKER : Entities::SHINING;
        fake.enPos = spawnPos;
        fake.isHallucination = true;
        fake.entity = entity(fake.type, sf::Color(255, 255, 255, 150));

        enData.push_back(fake);
    }
}