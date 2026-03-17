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

    float sanityDrainTimer = 0.f;
    float stalkerTimer = 0.f; // STALKER: cooldown before next teleport

    sf::Vector2f husklDir = { 1.f, 0.f };

    float animTimer = 0.f;
    int   animStep = 0;
    bool  facingRight = true;

    bool isHallucination = false;
	bool delEntity = false;
};

inline std::vector<EntityData> enData;

inline sf::VertexArray entity(Entities type, sf::Color c) {
    sf::VertexArray va(sf::PrimitiveType::Lines);

    if (type == Entities::LURKER) {
        va.append(sf::Vertex({ {   0.f,   0.f }, c })); va.append(sf::Vertex({ {  -4.f, -20.f }, c }));
        va.append(sf::Vertex({ {  -4.f, -20.f }, c })); va.append(sf::Vertex({ { -10.f, -42.f }, c }));
        va.append(sf::Vertex({ { -10.f, -42.f }, c })); va.append(sf::Vertex({ {  -6.f, -62.f }, c }));
        va.append(sf::Vertex({ {  -6.f, -62.f }, c })); va.append(sf::Vertex({ {  -2.f, -70.f }, c }));

        for (int i = 0; i < 6; ++i) {
            float a1 = pi + (i / 6.f) * pi, a2 = pi + ((i + 1) / 6.f) * pi;
            va.append(sf::Vertex({ { -2.f + std::cosf(a1) * 7.f, -78.f + std::sinf(a1) * 8.f }, c }));
            va.append(sf::Vertex({ { -2.f + std::cosf(a2) * 7.f, -78.f + std::sinf(a2) * 8.f }, c }));
        }
        for (int i = 0; i < 6; ++i) {
            float a1 = (i / 6.f) * pi, a2 = ((i + 1) / 6.f) * pi;
            va.append(sf::Vertex({ { -2.f + std::cosf(a1) * 7.f, -78.f + std::sinf(a1) * 8.f }, c }));
            va.append(sf::Vertex({ { -2.f + std::cosf(a2) * 7.f, -78.f + std::sinf(a2) * 8.f }, c }));
        }
        va.append(sf::Vertex({ { -7.f, -77.f }, c })); va.append(sf::Vertex({ { -4.f, -77.f }, c }));
        va.append(sf::Vertex({ {  1.f, -76.f }, c })); va.append(sf::Vertex({ {  4.f, -76.f }, c }));

        va.append(sf::Vertex({ {  -7.f, -54.f }, c })); va.append(sf::Vertex({ { -22.f, -34.f }, c }));
        va.append(sf::Vertex({ { -22.f, -34.f }, c })); va.append(sf::Vertex({ { -40.f, -10.f }, c }));
        va.append(sf::Vertex({ { -40.f, -10.f }, c })); va.append(sf::Vertex({ { -46.f,   6.f }, c }));

        va.append(sf::Vertex({ { -46.f,   6.f }, c })); va.append(sf::Vertex({ { -52.f,   2.f }, c }));
        va.append(sf::Vertex({ { -46.f,   6.f }, c })); va.append(sf::Vertex({ { -50.f,  13.f }, c }));
        va.append(sf::Vertex({ { -46.f,   6.f }, c })); va.append(sf::Vertex({ { -41.f,  13.f }, c }));

        va.append(sf::Vertex({ {  -3.f, -54.f }, c })); va.append(sf::Vertex({ {  16.f, -46.f }, c }));
        va.append(sf::Vertex({ {  16.f, -46.f }, c })); va.append(sf::Vertex({ {  28.f, -26.f }, c }));
        va.append(sf::Vertex({ {  28.f, -26.f }, c })); va.append(sf::Vertex({ {  32.f,  -8.f }, c }));

        va.append(sf::Vertex({ {  32.f,  -8.f }, c })); va.append(sf::Vertex({ {  38.f,  -4.f }, c }));
        va.append(sf::Vertex({ {  32.f,  -8.f }, c })); va.append(sf::Vertex({ {  36.f, -14.f }, c }));
        va.append(sf::Vertex({ {  32.f,  -8.f }, c })); va.append(sf::Vertex({ {  29.f,   1.f }, c }));

        va.append(sf::Vertex({ {   0.f,   0.f }, c })); va.append(sf::Vertex({ { -14.f,  16.f }, c }));
        va.append(sf::Vertex({ { -14.f,  16.f }, c })); va.append(sf::Vertex({ {  -8.f,  28.f }, c }));
        va.append(sf::Vertex({ {   0.f,   0.f }, c })); va.append(sf::Vertex({ {  10.f,  14.f }, c }));
        va.append(sf::Vertex({ {  10.f,  14.f }, c })); va.append(sf::Vertex({ {   6.f,  26.f }, c }));
    }
    else if (type == Entities::WRAITH) {
        for (int i = 0; i < 8; ++i) {
            float a1 = pi + (i / 8.f) * pi, a2 = pi + ((i + 1) / 8.f) * pi;
            va.append(sf::Vertex({ { std::cosf(a1) * 10.f, -14.f + std::sinf(a1) * 8.f }, c }));
            va.append(sf::Vertex({ { std::cosf(a2) * 10.f, -14.f + std::sinf(a2) * 8.f }, c }));
        }
        va.append(sf::Vertex({ { -5.f, -16.f }, c })); va.append(sf::Vertex({ { -2.f, -16.f }, c }));
        va.append(sf::Vertex({ {  2.f, -16.f }, c })); va.append(sf::Vertex({ {  5.f, -16.f }, c }));

        va.append(sf::Vertex({ { -17.f, -5.f }, c })); va.append(sf::Vertex({ {  13.f, -3.f }, c }));

        float sX[]  = { -17.f, -11.f,  -5.f,   1.f,   6.f,  10.f,  14.f };
        float sLen[] = {  40.f,  55.f,  30.f,  50.f,  36.f,  52.f,  26.f };
        float sDrft[] = {  -5.f,   3.f,  -3.f,   6.f,  -4.f,   2.f,  -2.f };
        for (int i = 0; i < 7; ++i) {
            va.append(sf::Vertex({ { sX[i],              -3.f            }, c }));
            va.append(sf::Vertex({ { sX[i] + sDrft[i],  -3.f + sLen[i]  }, c }));
        }
        va.append(sf::Vertex({ { -13.f,  16.f }, c })); va.append(sf::Vertex({ { -20.f,  14.f }, c }));
        va.append(sf::Vertex({ {   4.f,  24.f }, c })); va.append(sf::Vertex({ {  11.f,  20.f }, c }));
        va.append(sf::Vertex({ {  10.f,  34.f }, c })); va.append(sf::Vertex({ {  17.f,  30.f }, c }));
        va.append(sf::Vertex({ {  -7.f,  38.f }, c })); va.append(sf::Vertex({ { -13.f,  34.f }, c }));
    }
    else if (type == Entities::HOLLOW) {
        va.append(sf::Vertex({ {  -8.f,   0.f }, c })); va.append(sf::Vertex({ {   8.f,   0.f }, c }));

        va.append(sf::Vertex({ {  -8.f,   0.f }, c })); va.append(sf::Vertex({ {  -9.f, -30.f }, c }));
        va.append(sf::Vertex({ {   8.f,   0.f }, c })); va.append(sf::Vertex({ {   9.f, -30.f }, c }));

        va.append(sf::Vertex({ {  -9.f, -30.f }, c })); va.append(sf::Vertex({ { -12.f, -36.f }, c }));
        va.append(sf::Vertex({ {   9.f, -30.f }, c })); va.append(sf::Vertex({ {  12.f, -36.f }, c }));
        va.append(sf::Vertex({ { -12.f, -36.f }, c })); va.append(sf::Vertex({ {  -5.f, -38.f }, c }));
        va.append(sf::Vertex({ {  12.f, -36.f }, c })); va.append(sf::Vertex({ {   5.f, -38.f }, c }));

        va.append(sf::Vertex({ { -12.f, -36.f }, c })); va.append(sf::Vertex({ { -11.f, -62.f }, c }));
        va.append(sf::Vertex({ {  12.f, -36.f }, c })); va.append(sf::Vertex({ {  11.f, -62.f }, c }));

        va.append(sf::Vertex({ { -10.f, -43.f }, c })); va.append(sf::Vertex({ {  -4.f, -41.f }, c }));
        va.append(sf::Vertex({ {  10.f, -44.f }, c })); va.append(sf::Vertex({ {   4.f, -42.f }, c }));
        va.append(sf::Vertex({ { -10.f, -51.f }, c })); va.append(sf::Vertex({ {  -3.f, -49.f }, c }));
        va.append(sf::Vertex({ {  10.f, -51.f }, c })); va.append(sf::Vertex({ {   3.f, -50.f }, c }));
        va.append(sf::Vertex({ { -10.f, -58.f }, c })); va.append(sf::Vertex({ {  -4.f, -57.f }, c }));
        va.append(sf::Vertex({ {  10.f, -58.f }, c })); va.append(sf::Vertex({ {   4.f, -57.f }, c }));

        va.append(sf::Vertex({ { -11.f, -62.f }, c })); va.append(sf::Vertex({ { -20.f, -58.f }, c }));
        va.append(sf::Vertex({ {  11.f, -62.f }, c })); va.append(sf::Vertex({ {  20.f, -58.f }, c }));

        va.append(sf::Vertex({ { -20.f, -58.f }, c })); va.append(sf::Vertex({ { -18.f, -30.f }, c }));
        va.append(sf::Vertex({ {  20.f, -58.f }, c })); va.append(sf::Vertex({ {  18.f, -30.f }, c }));

        va.append(sf::Vertex({ {  -4.f, -62.f }, c })); va.append(sf::Vertex({ {   4.f, -62.f }, c }));
        va.append(sf::Vertex({ {   0.f, -62.f }, c })); va.append(sf::Vertex({ {   0.f, -70.f }, c }));

        for (int i = 0; i < 6; ++i) {
            float a1 = pi + (i / 6.f) * pi, a2 = pi + ((i + 1) / 6.f) * pi;
            va.append(sf::Vertex({ { std::cosf(a1) * 9.f, -78.f + std::sinf(a1) * 8.f }, c }));
            va.append(sf::Vertex({ { std::cosf(a2) * 9.f, -78.f + std::sinf(a2) * 8.f }, c }));
        }
        for (int i = 0; i < 6; ++i) {
            float a1 = (i / 6.f) * pi, a2 = ((i + 1) / 6.f) * pi;
            va.append(sf::Vertex({ { std::cosf(a1) * 9.f, -78.f + std::sinf(a1) * 8.f }, c }));
            va.append(sf::Vertex({ { std::cosf(a2) * 9.f, -78.f + std::sinf(a2) * 8.f }, c }));
        }
    }
    else if (type == Entities::STALKER) {
        for (int i = 0; i < 10; ++i) {
            float a1 = pi + (i / 10.f) * pi, a2 = pi + ((i + 1) / 10.f) * pi;
            va.append(sf::Vertex({ { std::cosf(a1) * 22.f, std::sinf(a1) * 9.f }, c }));
            va.append(sf::Vertex({ { std::cosf(a2) * 22.f, std::sinf(a2) * 9.f }, c }));
        }
        va.append(sf::Vertex({ { -22.f, 0.f }, c })); va.append(sf::Vertex({ {  22.f, 0.f }, c }));

        sf::Color eyeC = c; eyeC.a = std::min(255, static_cast<int>(c.a) + 60);
        va.append(sf::Vertex({ { -14.f, -5.f }, eyeC })); va.append(sf::Vertex({ { -10.f, -5.f }, eyeC }));
        va.append(sf::Vertex({ {  -6.f, -6.f }, eyeC })); va.append(sf::Vertex({ {  -2.f, -6.f }, eyeC }));
        va.append(sf::Vertex({ {   2.f, -6.f }, eyeC })); va.append(sf::Vertex({ {   6.f, -6.f }, eyeC }));
        va.append(sf::Vertex({ {  10.f, -5.f }, eyeC })); va.append(sf::Vertex({ {  14.f, -5.f }, eyeC }));

        va.append(sf::Vertex({ { -18.f,  -2.f }, c })); va.append(sf::Vertex({ { -30.f, -11.f }, c }));
        va.append(sf::Vertex({ { -30.f, -11.f }, c })); va.append(sf::Vertex({ { -36.f,   3.f }, c }));
        va.append(sf::Vertex({ { -17.f,   0.f }, c })); va.append(sf::Vertex({ { -29.f,   7.f }, c }));
        va.append(sf::Vertex({ { -15.f,   2.f }, c })); va.append(sf::Vertex({ { -26.f,  15.f }, c }));
        va.append(sf::Vertex({ {  18.f,  -2.f }, c })); va.append(sf::Vertex({ {  30.f, -11.f }, c }));
        va.append(sf::Vertex({ {  30.f, -11.f }, c })); va.append(sf::Vertex({ {  36.f,   3.f }, c }));
        va.append(sf::Vertex({ {  17.f,   0.f }, c })); va.append(sf::Vertex({ {  29.f,   7.f }, c }));
        va.append(sf::Vertex({ {  15.f,   2.f }, c })); va.append(sf::Vertex({ {  26.f,  15.f }, c }));
    }
    else if (type == Entities::HUSK) {
        va.append(sf::Vertex({ {  -6.f,   0.f }, c })); va.append(sf::Vertex({ {   4.f,   0.f }, c }));
        va.append(sf::Vertex({ {   7.f,   0.f }, c })); va.append(sf::Vertex({ {  15.f,  -2.f }, c }));

        va.append(sf::Vertex({ {  -4.f,   0.f }, c })); va.append(sf::Vertex({ {  -8.f, -22.f }, c }));
        va.append(sf::Vertex({ {  -8.f, -22.f }, c })); va.append(sf::Vertex({ {  -2.f, -40.f }, c }));

        va.append(sf::Vertex({ {   9.f,   0.f }, c })); va.append(sf::Vertex({ {   9.f, -38.f }, c }));

        va.append(sf::Vertex({ {  -2.f, -40.f }, c })); va.append(sf::Vertex({ {   9.f, -38.f }, c }));

        va.append(sf::Vertex({ {   4.f, -38.f }, c })); va.append(sf::Vertex({ {   2.f, -46.f }, c }));
        va.append(sf::Vertex({ {   2.f, -46.f }, c })); va.append(sf::Vertex({ {   4.f, -54.f }, c }));
        va.append(sf::Vertex({ {   4.f, -54.f }, c })); va.append(sf::Vertex({ {   1.f, -62.f }, c }));
        va.append(sf::Vertex({ {   1.f, -62.f }, c })); va.append(sf::Vertex({ {   2.f, -70.f }, c }));

        va.append(sf::Vertex({ {   1.f, -46.f }, c })); va.append(sf::Vertex({ { -10.f, -44.f }, c }));
        va.append(sf::Vertex({ {   2.f, -46.f }, c })); va.append(sf::Vertex({ {  11.f, -43.f }, c }));
        va.append(sf::Vertex({ {   2.f, -52.f }, c })); va.append(sf::Vertex({ {  -9.f, -50.f }, c }));
        va.append(sf::Vertex({ {   3.f, -52.f }, c })); va.append(sf::Vertex({ {  10.f, -49.f }, c }));
        va.append(sf::Vertex({ {   2.f, -58.f }, c })); va.append(sf::Vertex({ {  -8.f, -57.f }, c }));
        va.append(sf::Vertex({ {   3.f, -58.f }, c })); va.append(sf::Vertex({ {   9.f, -56.f }, c }));

        va.append(sf::Vertex({ {   0.f, -62.f }, c })); va.append(sf::Vertex({ { -14.f, -58.f }, c }));
        va.append(sf::Vertex({ { -14.f, -58.f }, c })); va.append(sf::Vertex({ { -18.f, -42.f }, c }));
        va.append(sf::Vertex({ { -18.f, -42.f }, c })); va.append(sf::Vertex({ { -20.f, -28.f }, c }));

        va.append(sf::Vertex({ {   2.f, -62.f }, c })); va.append(sf::Vertex({ {  16.f, -66.f }, c }));
        va.append(sf::Vertex({ {  16.f, -66.f }, c })); va.append(sf::Vertex({ {  20.f, -52.f }, c }));
        va.append(sf::Vertex({ {  20.f, -52.f }, c })); va.append(sf::Vertex({ {  22.f, -40.f }, c }));

        va.append(sf::Vertex({ {  22.f, -40.f }, c })); va.append(sf::Vertex({ {  28.f, -35.f }, c }));
        va.append(sf::Vertex({ {  22.f, -40.f }, c })); va.append(sf::Vertex({ {  27.f, -33.f }, c }));
        va.append(sf::Vertex({ {  22.f, -40.f }, c })); va.append(sf::Vertex({ {  20.f, -33.f }, c }));

        va.append(sf::Vertex({ {  -5.f, -70.f }, c })); va.append(sf::Vertex({ {   8.f, -70.f }, c }));
        va.append(sf::Vertex({ {  -5.f, -70.f }, c })); va.append(sf::Vertex({ {  -7.f, -80.f }, c }));
        va.append(sf::Vertex({ {   8.f, -70.f }, c })); va.append(sf::Vertex({ {   8.f, -80.f }, c }));
        va.append(sf::Vertex({ {  -7.f, -80.f }, c })); va.append(sf::Vertex({ {   8.f, -80.f }, c }));

        va.append(sf::Vertex({ {  -4.f, -75.f }, c })); va.append(sf::Vertex({ {  -1.f, -75.f }, c }));
        va.append(sf::Vertex({ {   2.f, -75.f }, c })); va.append(sf::Vertex({ {   5.f, -75.f }, c }));

        va.append(sf::Vertex({ {  -3.f, -70.f }, c })); va.append(sf::Vertex({ {  -4.f, -64.f }, c }));
        va.append(sf::Vertex({ {   6.f, -70.f }, c })); va.append(sf::Vertex({ {   7.f, -64.f }, c }));
        va.append(sf::Vertex({ {  -4.f, -64.f }, c })); va.append(sf::Vertex({ {   5.f, -63.f }, c }));
    }
    else if (type == Entities::LEECH) {
        int bSeg = 16;
        float rx = 20.f, ry = 9.f;
        for (int i = 0; i < bSeg; ++i) {
            float a1 = (i / static_cast<float>(bSeg)) * 2.f * pi;
            float a2 = ((i + 1) / static_cast<float>(bSeg)) * 2.f * pi;
            va.append(sf::Vertex({ { std::cosf(a1) * rx, std::sinf(a1) * ry }, c }));
            va.append(sf::Vertex({ { std::cosf(a2) * rx, std::sinf(a2) * ry }, c }));
        }

        for (int i = -2; i <= 2; ++i) {
            float xOff  = i * 6.f;
            float halfH = ry * std::sqrtf(std::max(0.f, 1.f - (xOff * xOff) / (rx * rx)));
            va.append(sf::Vertex({ { xOff, -halfH }, c }));
            va.append(sf::Vertex({ { xOff,  halfH }, c }));
        }

        float mR = 5.f, mX = 18.f;
        for (int i = 0; i < 6; ++i) {
            float a1 = (i / 6.f) * 2.f * pi, a2 = ((i + 1) / 6.f) * 2.f * pi;
            va.append(sf::Vertex({ { mX + std::cosf(a1) * mR, std::sinf(a1) * mR }, c }));
            va.append(sf::Vertex({ { mX + std::cosf(a2) * mR, std::sinf(a2) * mR }, c }));
        }
        va.append(sf::Vertex({ { mX - mR,  0.f }, c })); va.append(sf::Vertex({ { mX + mR,  0.f }, c }));
        va.append(sf::Vertex({ { mX,      -mR  }, c })); va.append(sf::Vertex({ { mX,       mR  }, c }));

        sf::Color eyeC = sf::Color::Red;
        va.append(sf::Vertex({ {  9.f, -5.f }, eyeC })); va.append(sf::Vertex({ { 13.f, -5.f }, eyeC }));
        va.append(sf::Vertex({ {  9.f,  5.f }, eyeC })); va.append(sf::Vertex({ { 13.f,  5.f }, eyeC }));
    }
    else if (type == Entities::SHINING) {
        float sSize = 26.f;
        int seg = 14;

        for (int i = 0; i < seg; ++i) {
            float a1 = (i / static_cast<float>(seg)) * 2.f * pi;
            float a2 = ((i + 1) / static_cast<float>(seg)) * 2.f * pi;
            va.append(sf::Vertex({ { std::cosf(a1) * 8.f, std::sinf(a1) * 8.f }, c }));
            va.append(sf::Vertex({ { std::cosf(a2) * 8.f, std::sinf(a2) * 8.f }, c }));
        }
        for (int i = 0; i < seg; ++i) {
            float a1 = (i / static_cast<float>(seg)) * 2.f * pi;
            float a2 = ((i + 1) / static_cast<float>(seg)) * 2.f * pi;
            va.append(sf::Vertex({ { std::cosf(a1) * sSize * 0.55f, std::sinf(a1) * sSize * 0.55f }, c }));
            va.append(sf::Vertex({ { std::cosf(a2) * sSize * 0.55f, std::sinf(a2) * sSize * 0.55f }, c }));
        }
        for (int i = 0; i < seg; ++i) {
            float a1 = (i / static_cast<float>(seg)) * 2.f * pi;
            float a2 = ((i + 1) / static_cast<float>(seg)) * 2.f * pi;
            va.append(sf::Vertex({ { std::cosf(a1) * sSize, std::sinf(a1) * sSize }, c }));
            va.append(sf::Vertex({ { std::cosf(a2) * sSize, std::sinf(a2) * sSize }, c }));
        }
        for (int i = 0; i < 8; ++i) {
            float a = (i / 8.f) * 2.f * pi;
            va.append(sf::Vertex({ { std::cosf(a) * sSize,         std::sinf(a) * sSize         }, c }));
            va.append(sf::Vertex({ { std::cosf(a) * (sSize + 14.f), std::sinf(a) * (sSize + 14.f) }, c }));
        }
        for (int i = 0; i < 8; ++i) {
            float a = (pi / 8.f) + (i / 8.f) * 2.f * pi;
            va.append(sf::Vertex({ { std::cosf(a) * sSize,        std::sinf(a) * sSize        }, c }));
            va.append(sf::Vertex({ { std::cosf(a) * (sSize + 7.f), std::sinf(a) * (sSize + 7.f) }, c }));
        }
        va.append(sf::Vertex({ { -7.f,  0.f }, c })); va.append(sf::Vertex({ {  7.f,  0.f }, c }));
        va.append(sf::Vertex({ {  0.f, -7.f }, c })); va.append(sf::Vertex({ {  0.f,  7.f }, c }));
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