#pragma once
#include "../../Misc/Functions.hpp"
#include "../../Misc/Includes.hpp"
#include "../../Misc/Variables.hpp"

struct EntityData {
	sf::VertexArray entity;
	Entities type;
	sf::Vector2f enPos;
    sf::Vector2f patPos;
    sf::Vector2i enChunk;

	int sDrain = 1;
	int damage = 2;
	int damageP = 0;

    float suspicion = 0.f;
    float stunTimer = 0.f;

    float orbitAngle = 0.f;
    float orbitRadius = 0.f;

    float stalkerTimer = 0.f; // STALKER: cooldown before next teleport

    sf::Vector2f husklDir = { 1.f, 0.f };

    bool isHallucination = false;
	bool delEntity = false;
};

inline std::vector<EntityData> enData;

inline sf::VertexArray entity(Entities type, sf::Color c) {
    sf::VertexArray va(sf::PrimitiveType::Lines);

    if (type == Entities::LURKER) {
        va.append(sf::Vertex({ {  0.f,   0.f }, c })); va.append(sf::Vertex({ { -7.f,  -28.f }, c })); // left leg
        va.append(sf::Vertex({ {  0.f,   0.f }, c })); va.append(sf::Vertex({ {  7.f,  -28.f }, c })); // right leg

        va.append(sf::Vertex({ { -7.f, -28.f }, c })); va.append(sf::Vertex({ {  7.f,  -28.f }, c }));

        va.append(sf::Vertex({ {  0.f, -28.f }, c })); va.append(sf::Vertex({ {  0.f,  -68.f }, c }));

        va.append(sf::Vertex({ { -14.f, -65.f }, c })); va.append(sf::Vertex({ {  14.f, -65.f }, c }));

        va.append(sf::Vertex({ { -14.f, -65.f }, c })); va.append(sf::Vertex({ { -18.f, -18.f }, c })); // left arm
        va.append(sf::Vertex({ {  14.f, -65.f }, c })); va.append(sf::Vertex({ {  18.f, -18.f }, c })); // right arm

        va.append(sf::Vertex({ {  0.f, -68.f }, c })); va.append(sf::Vertex({ {  0.f,  -76.f }, c }));

        for (int i = 0; i < 4; ++i) {
            float a1 = pi + (i / 4.f) * pi;
            float a2 = pi + ((i + 1) / 4.f) * pi;
            va.append(sf::Vertex({ { std::cos(a1) * 6.f,        -76.f + std::sin(a1) * 8.f }, c }));
            va.append(sf::Vertex({ { std::cos(a2) * 6.f,        -76.f + std::sin(a2) * 8.f }, c }));
        }
        for (int i = 0; i < 4; ++i) {
            float a1 = (i / 4.f) * pi;
            float a2 = ((i + 1) / 4.f) * pi;
            va.append(sf::Vertex({ { std::cos(a1) * 6.f,        -76.f + std::sin(a1) * 8.f }, c }));
            va.append(sf::Vertex({ { std::cos(a2) * 6.f,        -76.f + std::sin(a2) * 8.f }, c }));
        }
    }
    else if (type == Entities::WRAITH) {
        int arcSeg = 10;
        float crx = 16.f, cry = 7.f;
        for (int i = 0; i < arcSeg; ++i) {
            float a1 = pi + (i / static_cast<float>(arcSeg)) * pi;
            float a2 = pi + ((i + 1) / static_cast<float>(arcSeg)) * pi;
            va.append(sf::Vertex({ { std::cos(a1) * crx, std::sin(a1) * cry - 2.f }, c }));
            va.append(sf::Vertex({ { std::cos(a2) * crx, std::sin(a2) * cry - 2.f }, c }));
        }
        float strandX[]  = { -14.f, -9.f, -4.f,  1.f,  6.f, 11.f, 15.f };
        float strandLen[] = {  22.f, 30.f, 18.f, 34.f, 20.f, 28.f, 16.f };
        for (int i = 0; i < 7; ++i) {
            va.append(sf::Vertex({ { strandX[i], -2.f              }, c }));
            va.append(sf::Vertex({ { strandX[i], -2.f + strandLen[i] }, c }));
        }
        va.append(sf::Vertex({ { -6.f, 1.f }, c })); va.append(sf::Vertex({ { -3.f, 1.f }, c }));
        va.append(sf::Vertex({ {  3.f, 1.f }, c })); va.append(sf::Vertex({ {  6.f, 1.f }, c }));
    }
    else if (type == Entities::HOLLOW) {
        va.append(sf::Vertex({ { -5.f,  0.f }, c })); va.append(sf::Vertex({ {  5.f,  0.f }, c }));
        va.append(sf::Vertex({ { -5.f,  0.f }, c })); va.append(sf::Vertex({ { -6.f, -22.f }, c }));
        va.append(sf::Vertex({ {  5.f,  0.f }, c })); va.append(sf::Vertex({ {  6.f, -22.f }, c }));

        va.append(sf::Vertex({ { -6.f, -22.f }, c })); va.append(sf::Vertex({ {  6.f, -22.f }, c }));

        va.append(sf::Vertex({ { -6.f, -22.f }, c })); va.append(sf::Vertex({ { -13.f, -50.f }, c }));
        va.append(sf::Vertex({ {  6.f, -22.f }, c })); va.append(sf::Vertex({ {  13.f, -50.f }, c }));

        va.append(sf::Vertex({ { -13.f, -50.f }, c })); va.append(sf::Vertex({ {  13.f, -50.f }, c }));

        va.append(sf::Vertex({ { -13.f, -50.f }, c })); va.append(sf::Vertex({ { -18.f, -36.f }, c }));
        va.append(sf::Vertex({ {  13.f, -50.f }, c })); va.append(sf::Vertex({ {  18.f, -36.f }, c }));

        va.append(sf::Vertex({ { -3.f, -50.f }, c })); va.append(sf::Vertex({ { -4.f, -57.f }, c }));
        va.append(sf::Vertex({ {  3.f, -50.f }, c })); va.append(sf::Vertex({ {  4.f, -57.f }, c }));
        va.append(sf::Vertex({ { -4.f, -57.f }, c })); va.append(sf::Vertex({ {  4.f, -57.f }, c }));
    }
    else if (type == Entities::STALKER) {
        int bSeg = 6;
        for (int i = 0; i < bSeg; ++i) {
            float a1 = (i / static_cast<float>(bSeg)) * pi;
            float a2 = ((i + 1) / static_cast<float>(bSeg)) * pi;
            va.append(sf::Vertex({ { std::cos(a1) * 18.f, std::sin(a1) * 8.f }, c }));
            va.append(sf::Vertex({ { std::cos(a2) * 18.f, std::sin(a2) * 8.f }, c }));
        }
        for (int i = 0; i < 5; ++i) {
            float a1 = pi + (i / 5.f) * pi;
            float a2 = pi + ((i + 1) / 5.f) * pi;
            va.append(sf::Vertex({ { -10.f + std::cos(a1) * 9.f, -6.f + std::sin(a1) * 9.f }, c }));
            va.append(sf::Vertex({ { -10.f + std::cos(a2) * 9.f, -6.f + std::sin(a2) * 9.f }, c }));
        }
        for (int i = 0; i < 5; ++i) {
            float a1 = pi + (i / 5.f) * pi;
            float a2 = pi + ((i + 1) / 5.f) * pi;
            va.append(sf::Vertex({ { std::cos(a1) * 10.f, -10.f + std::sin(a1) * 10.f }, c }));
            va.append(sf::Vertex({ { std::cos(a2) * 10.f, -10.f + std::sin(a2) * 10.f }, c }));
        }
        for (int i = 0; i < 5; ++i) {
            float a1 = pi + (i / 5.f) * pi;
            float a2 = pi + ((i + 1) / 5.f) * pi;
            va.append(sf::Vertex({ { 10.f + std::cos(a1) * 8.f, -4.f + std::sin(a1) * 8.f }, c }));
            va.append(sf::Vertex({ { 10.f + std::cos(a2) * 8.f, -4.f + std::sin(a2) * 8.f }, c }));
        }
        sf::Color eyeC = c; eyeC.a = std::min(255, static_cast<int>(c.a) + 40);
        va.append(sf::Vertex({ { -5.f, -4.f }, eyeC })); va.append(sf::Vertex({ { -2.f, -4.f }, eyeC }));
        va.append(sf::Vertex({ {  2.f, -4.f }, eyeC })); va.append(sf::Vertex({ {  5.f, -4.f }, eyeC }));
    }
    else if (type == Entities::HUSK) {
        va.append(sf::Vertex({ {  3.f,  0.f }, c })); va.append(sf::Vertex({ {  4.f, -16.f }, c }));

        va.append(sf::Vertex({ { -3.f,  0.f }, c })); va.append(sf::Vertex({ { -14.f, 4.f }, c }));

        va.append(sf::Vertex({ { -3.f,  0.f }, c })); va.append(sf::Vertex({ {  3.f,  0.f }, c }));

        va.append(sf::Vertex({ {  0.f,  0.f }, c })); va.append(sf::Vertex({ { -4.f, -30.f }, c }));

        va.append(sf::Vertex({ { -4.f, -24.f }, c })); va.append(sf::Vertex({ { -12.f, -14.f }, c }));

        va.append(sf::Vertex({ { -4.f, -24.f }, c })); va.append(sf::Vertex({ {  4.f,  -20.f }, c }));

        va.append(sf::Vertex({ { -4.f, -30.f }, c })); va.append(sf::Vertex({ { -3.f, -36.f }, c }));
        va.append(sf::Vertex({ { -3.f, -36.f }, c })); va.append(sf::Vertex({ {  2.f, -34.f }, c })); // jaw slump
        va.append(sf::Vertex({ { -3.f, -36.f }, c })); va.append(sf::Vertex({ { -5.f, -42.f }, c })); // crown
    }
    else if (type == Entities::LEECH) {
        float sz = 10.f;

        va.append(sf::Vertex({ { -sz, 0 }, c })); va.append(sf::Vertex({ { 0, -sz }, c }));
        va.append(sf::Vertex({ { 0, -sz }, c })); va.append(sf::Vertex({ { sz, 0 }, c }));

        sf::Color eyeC = sf::Color::Red;
        va.append(sf::Vertex({ { -4, -5 }, eyeC })); va.append(sf::Vertex({ { -1, -5 }, eyeC }));
        va.append(sf::Vertex({ { 1, -5 }, eyeC })); va.append(sf::Vertex({ { 4, -5 }, eyeC }));
    }
    else if (type == Entities::SHINING) {
        int segments = 12;
        float sSize = 25.f;
        for (int i = 0; i < segments; ++i) {
            float a1 = (i / static_cast<float>(segments)) * pi;
            float a2 = ((i + 1) / static_cast<float>(segments)) * pi;
            va.append(sf::Vertex({ { std::cosf(a1) * sSize, std::sinf(a1) * (sSize * 0.5f) }, c }));
            va.append(sf::Vertex({ { std::cosf(a2) * sSize, std::sinf(a2) * (sSize * 0.5f) }, c }));
        }
    }
    return va;
}

inline void sEntity(Entities type, sf::Vector2f pos, sf::Vector2i chPos) {
    EntityData data;
    data.type = type;
    data.enPos = pos;
    data.enChunk = chPos;

    sf::Color spawnCol = (type == Entities::STALKER)
        ? sf::Color(255, 255, 255, 55)
        : sf::Color::White;

    data.entity = entity(type, spawnCol);

    if (type == Entities::HUSK) {
        float angle = static_cast<float>(std::rand() % 360) * pi / 180.f;
        data.husklDir = { std::cos(angle), std::sin(angle) };
    }
    if (type == Entities::WRAITH) {
        data.orbitAngle = static_cast<float>(std::rand() % 360) * pi / 180.f;
        data.orbitRadius = chSize * 3.f;
    }
    if (type == Entities::STALKER) {
        data.stalkerTimer = 3.f;
    }

    enData.push_back(data);
}

inline void uPatrol(EntityData& en, float dt) {
    float patDist = std::sqrt(std::pow(en.patPos.x - en.enPos.x, 2) + std::pow(en.patPos.y - en.enPos.y, 2));

    if (patDist < 20.f || (en.patPos.x == 0 && en.patPos.y == 0)) {
        int offX = (std::rand() % 3) - 1;
        int offY = (std::rand() % 3) - 1;

        sf::Vector2i patCh = en.enChunk + sf::Vector2i(offX, offY);
        en.patPos = sf::Vector2f(
            patCh.x * chSize + static_cast<float>(std::rand() % static_cast<int>(chSize)),
            patCh.y * chSize + static_cast<float>(std::rand() % static_cast<int>(chSize))
        );
    }

    if (en.suspicion < 1.f) {
        sf::Vector2f diff = en.patPos - en.enPos;
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        if (dist > 0.1f) {
            en.enPos += (diff / dist) * 40.f * dt;
        }
    }
}