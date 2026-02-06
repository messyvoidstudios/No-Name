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

inline void dTree(sf::VertexArray& tree, sf::Vector2f pos, float scale) {
    sf::Color c = sf::Color(40, 40, 40);

    tree.append(sf::Vertex({ { pos.x, pos.y }, c }));
    tree.append(sf::Vertex({ { pos.x, pos.y - (20.f * scale) }, c }));

    tree.append(sf::Vertex({ { pos.x, pos.y - (10.f * scale) }, c }));
    tree.append(sf::Vertex({ { pos.x - (10.f * scale), pos.y - (18.f * scale) }, c }));

    tree.append(sf::Vertex({ { pos.x, pos.y - (15.f * scale) }, c }));
    tree.append(sf::Vertex({ { pos.x + (8.f * scale), pos.y - (22.f * scale) }, c }));
}

inline void dGrass(sf::VertexArray& grass, sf::Vector2f pos, float scale) {
    sf::Color grassC(20, 20, 20);
    grass.append(sf::Vertex({ { pos.x, pos.y }, grassC }));
    grass.append(sf::Vertex({ { pos.x + rand(-3.f, 3.f), pos.y - scale }, grassC }));
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

    int treeMax = 3;
    float treeScaleM = 0.7f;
    sf::Color forestColor = sf::Color(40, 40, 40);

    if (type == Chunks::FOREST) {
        treeMax = 15;
        treeScaleM = 1.0f;
    }

    int trees = 2 + (std::rand() % treeMax);
    for (int i = 0; i < trees; ++i) {
        sf::Vector2f randPos(rand(10.f, size - 10.f), rand(10.f, size - 10.f));

        if (type == Chunks::CAMP && std::abs(randPos.x - size / 2.f) < 60) continue;

        float s = rand(treeScaleM, 1.5f);
        dTree(ch, randPos, s);
    }

    int grassMult = (type == Chunks::FOREST) ? 3 : 1;
    int grassBlades = (15 + (std::rand() % 20)) * grassMult;
    for (int i = 0; i < grassBlades; ++i) {
        sf::Vector2f gPos(rand(5.f, size - 5.f), rand(5.f, size - 5.f));
        dGrass(ch, gPos, rand(5.f, 12.f));
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

                int neighbours = 0;
                for (const auto& existing : activeChunks) {
                    if (existing.type == Chunks::FOREST) {
                        if (std::abs(existing.chPos.x - x) <= 1 && std::abs(existing.chPos.y - y) <= 1) {
                            neighbours++;
                        }
                    }
                }

                float r = rand(0.f, 100.f);
                float fChance = (neighbours > 0) ? 40.f : 97.f;
                if (r > 99.5f) t = Chunks::PIT;
                else if (r > 98.5f) t = Chunks::CAMP;
                else if (r > fChance) t = Chunks::FOREST;

                activeChunks.push_back({ chunk(t, chSize, sf::Color::Transparent), t, {x, y} });
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