#pragma once
#include "../../../Misc/Functions.hpp"
#include "../../../Misc/Includes.hpp"
#include "../../../Misc/Variables.hpp"

struct ChunkData {
    sf::VertexArray chunk;
    Chunks type;
    sf::Vector2i chPos;
    bool isTriggered = false;
};

struct ChunkBounds {
    float minX, minY, maxX, maxY;
};

inline ChunkBounds cBounds;

inline std::vector<ChunkData> activeChunks;

inline void dTree(sf::VertexArray& va, sf::Vector2f pos, float scale) {
    sf::Color c = sf::Color(40, 40, 40);

    va.append(sf::Vertex({ { pos.x, pos.y }, c }));
    va.append(sf::Vertex({ { pos.x, pos.y - (20.f * scale) }, c }));

    va.append(sf::Vertex({ { pos.x, pos.y - (10.f * scale) }, c }));
    va.append(sf::Vertex({ { pos.x - (10.f * scale), pos.y - (18.f * scale) }, c }));

    va.append(sf::Vertex({ { pos.x, pos.y - (15.f * scale) }, c }));
    va.append(sf::Vertex({ { pos.x + (8.f * scale), pos.y - (22.f * scale) }, c }));
}

inline sf::VertexArray chunk(Chunks type, float size, sf::Color c) {
    sf::VertexArray ch(sf::PrimitiveType::Lines);

    sf::Color bColour = c;
    bColour.a = 0;

    ch.append(sf::Vertex({ { 0, 0 }, bColour })); ch.append(sf::Vertex({ { size, 0 }, bColour }));
    ch.append(sf::Vertex({ { size, 0 }, bColour })); ch.append(sf::Vertex({ { size, size }, bColour }));
    ch.append(sf::Vertex({ { size, size }, bColour })); ch.append(sf::Vertex({ { 0, size }, bColour }));
    ch.append(sf::Vertex({ { 0, size }, bColour })); ch.append(sf::Vertex({ { 0, 0 }, bColour }));

    if (type == Chunks::PIT) {
        sf::Color pitGrey(20, 20, 20);
        for (float i = 0; i <= size; i += 40.f) {
            ch.append(sf::Vertex({ { i, 0.f }, pitGrey }));
            ch.append(sf::Vertex({ { 0.f, i }, pitGrey }));
        }
    }

    else if (type == Chunks::CAMP) {
        sf::Color campC = sf::Color::White;
        sf::Color fireC = sf::Color(70, 70, 70);
        float mid = size / 2.f;

        ch.append(sf::Vertex({ { mid - 30, mid + 20 }, campC })); ch.append(sf::Vertex({ { mid, mid - 20 }, campC }));
        ch.append(sf::Vertex({ { mid, mid - 20 }, campC })); ch.append(sf::Vertex({ { mid + 30, mid + 20 }, campC }));
        ch.append(sf::Vertex({ { mid + 30, mid + 20 }, campC })); ch.append(sf::Vertex({ { mid - 30, mid + 20 }, campC }));

        float fx = mid + 50; float fy = mid + 10;
        ch.append(sf::Vertex({ { fx, fy - 10 }, fireC })); ch.append(sf::Vertex({ { fx + 10, fy }, fireC }));
        ch.append(sf::Vertex({ { fx + 10, fy }, fireC })); ch.append(sf::Vertex({ { fx, fy + 10 }, fireC }));
        ch.append(sf::Vertex({ { fx, fy + 10 }, fireC })); ch.append(sf::Vertex({ { fx - 10, fy }, fireC }));
        ch.append(sf::Vertex({ { fx - 10, fy }, fireC })); ch.append(sf::Vertex({ { fx, fy - 10 }, fireC }));
    }

    int trees = 2 + (std::rand() % 3);
    for (int i = 0; i < trees; ++i) {
        sf::Vector2f randPos(rand(20.f, size - 20.f), rand(20.f, size - 20.f));
        if (type == Chunks::CAMP && std::abs(randPos.x - size / 2.f) < 60) continue;

        dTree(ch, randPos, rand(0.7f, 1.2f));
    }

    return ch;
}

void rChunks(sf::Vector2i chPos) {
    int startX = chPos.x - rDist;
    int startY = chPos.y - rDist;
    int endX = chPos.x + rDist;
    int endY = chPos.y + rDist;

    auto it = activeChunks.begin();
    while (it != activeChunks.end()) {
        if (it->chPos.x < startX || it->chPos.x > endX ||
            it->chPos.y < startY || it->chPos.y > endY) {
            it = activeChunks.erase(it);
        }
        else {
            ++it;
        }
    }

    for (int x = startX; x <= endX; ++x) {
        for (int y = startY; y <= endY; ++y) {

            bool exists = false;
            for (const auto& c : activeChunks) {
                if (c.chPos.x == x && c.chPos.y == y) {
                    exists = true;
                    break;
                }
            }

            if (!exists) {
                Chunks t = Chunks::BASIC;
                float r = rand(0.f, 100.f);
                if (r > 99) t = Chunks::PIT;
                else if (r > 98) t = Chunks::CAMP;

                activeChunks.push_back({ chunk(t, chSize, sf::Color::White), t, {x, y} });
            }
        }
    }

    cBounds.minX = chPos.x * chSize;
    cBounds.minY = chPos.y * chSize;
    cBounds.maxX = (chPos.x + 1) * chSize;
    cBounds.maxY = (chPos.y + 1) * chSize;
}

inline void pit(sf::VertexArray& cir, float size, sf::Color c) {
    cir.clear();
    int segments = 16;
    float rad = size * 0.4f;
    sf::Vector2f centre(size / 2.f, size / 2.f);

    for (int i = 0; i < segments; ++i) {
        float a1 = (i / (float)segments) * 2.f * pi;
        float a2 = ((i + 1) / (float)segments) * 2.f * pi;

        cir.append(sf::Vertex({ centre + sf::Vector2f(std::cos(a1) * rad, std::sin(a1) * rad), c }));
        cir.append(sf::Vertex({ centre + sf::Vector2f(std::cos(a2) * rad, std::sin(a2) * rad), c }));
    }
}

inline void uChunks(sf::Vector2f playerPos) {
    sf::Vector2i curChunk(static_cast<int>(std::floor(playerPos.x / chSize)), static_cast<int>(std::floor(playerPos.y / chSize)));

    for (auto& c : activeChunks) {
        if (c.type == Chunks::PIT && !c.isTriggered) {
            if (curChunk == c.chPos) {
                c.isTriggered = true;
                pit(c.chunk, chSize, sf::Color::White);
            }
        }
    }
}