#pragma once
#include "../../Misc/Functions.hpp"
#include "../../Misc/Includes.hpp"
#include "../../Misc/Variables.hpp"

enum class Entities;
void sEntity(Entities type, sf::Vector2f pos, sf::Vector2i chPos);

struct ChunkData {
    sf::VertexArray chunk;
    sf::VertexArray eyes;

    Chunks type;

    sf::Vector2i chPos;
    sf::Vector2f eyePos;

    bool isTriggered = false;
    bool discovered = false;
    bool showEyes = false;
};

struct ChunkBounds {
    float minX, minY, maxX, maxY;
};

inline ChunkBounds chBounds;

inline std::vector<ChunkData> chData;

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

inline void dEyes(sf::VertexArray& eye, sf::Vector2f pos) {
    sf::Color eyeColor = sf::Color::White;
    float w = 3.f;
    float gap = 6.f;

    eye.append(sf::Vertex({ { pos.x - gap, pos.y - w }, eyeColor }));
    eye.append(sf::Vertex({ { pos.x - gap, pos.y + w }, eyeColor }));

    eye.append(sf::Vertex({ { pos.x + gap, pos.y - w }, eyeColor }));
    eye.append(sf::Vertex({ { pos.x + gap, pos.y + w }, eyeColor }));
}

inline void eNoise(sf::VertexArray& line, sf::Vector2f start, sf::Vector2f end, sf::Color c, int detail) {
    sf::Vector2f origin = start;
    for (int i = 1; i <= detail; ++i) {
        float t = (float)i / detail;
        sf::Vector2f pt = start + t * (end - start);

        if (i < detail) {
            pt.x += rand(-5.f, 5.f);
            pt.y += rand(-5.f, 5.f);
        }

        line.append(sf::Vertex({ origin, c }));
        line.append(sf::Vertex({ pt, c }));
        origin = pt;
    }
}

inline sf::VertexArray chunk(Chunks type, float size, sf::Color c) {
    sf::VertexArray ch(sf::PrimitiveType::Lines);
    float mid = size / 2.f;

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

        ch.append(sf::Vertex({ { mid - 30, mid + 20 }, campC })); ch.append(sf::Vertex({ { mid, mid - 20 }, campC }));
        ch.append(sf::Vertex({ { mid, mid - 20 }, campC })); ch.append(sf::Vertex({ { mid + 30, mid + 20 }, campC }));
        ch.append(sf::Vertex({ { mid + 30, mid + 20 }, campC })); ch.append(sf::Vertex({ { mid - 30, mid + 20 }, campC }));

        float fx = mid + 50; float fy = mid + 10;
        ch.append(sf::Vertex({ { fx, fy - 10 }, fireC })); ch.append(sf::Vertex({ { fx + 10, fy }, fireC }));
        ch.append(sf::Vertex({ { fx + 10, fy }, fireC })); ch.append(sf::Vertex({ { fx, fy + 10 }, fireC }));
        ch.append(sf::Vertex({ { fx, fy + 10 }, fireC })); ch.append(sf::Vertex({ { fx - 10, fy }, fireC }));
        ch.append(sf::Vertex({ { fx - 10, fy }, fireC })); ch.append(sf::Vertex({ { fx, fy - 10 }, fireC }));
    }

    else if (type == Chunks::ENTRANCE) {
        float groundY = size * 0.75f;

        int layer = 6;
        int segments = 12;
        float bRad = 55.f;

        for (int i = 0; i < layer; ++i) {
            float colorFactor = 1.0f - (static_cast<float>(i) / layer);
            uint8_t col = static_cast<uint8_t>(30 + (225 * colorFactor));
            uint8_t a = static_cast<uint8_t>(50 + (205 * colorFactor));

            sf::Color layerC(col, col, col, a);

            float scale = 1.0f - (i * 0.14f);
            float rad = bRad * scale;

            sf::Vector2f origin;

            for (int j = 0; j <= segments; ++j) {
                float angle = pi + (static_cast<float>(j) / segments) * pi;

                sf::Vector2f pt(mid + std::cos(angle) * rad, groundY + std::sin(angle) * rad);

                if (j > 0 && j < segments) {
                    pt.x += rand(-2.f, 2.f);
                    pt.y += rand(-2.f, 2.f);
                }

                if (j > 0) {
                    ch.append(sf::Vertex({ origin, layerC }));
                    ch.append(sf::Vertex({ pt, layerC }));
                }
                origin = pt;
            }
        }
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
        sf::Vector2f tPos(rand(10.f, size - 10.f), rand(10.f, size - 10.f));
        if (type == Chunks::CAMP && std::abs(tPos.x - size / 2.f) < 60) continue;
        if (type == Chunks::ENTRANCE && std::abs(tPos.x - size / 2.f) < 60) continue;
        dTree(ch, tPos, rand(treeScaleM, 1.5f));
    }

    int grassMult = (type == Chunks::FOREST) ? 3 : 1;
    int grassBlades = (15 + (std::rand() % 20)) * grassMult;
    for (int i = 0; i < grassBlades; ++i) {
        sf::Vector2f gPos(rand(5.f, size - 5.f), rand(5.f, size - 5.f));
        if (type == Chunks::ENTRANCE && std::abs(gPos.x - size / 2.f) < 60) continue;
        dGrass(ch, gPos, rand(5.f, 12.f));
    }
    return ch;
}

void rChunks(sf::Vector2i chPos) {
    int startX = chPos.x - rDist;
    int startY = chPos.y - rDist;
    int endX = chPos.x + rDist;
    int endY = chPos.y + rDist;

    auto it = chData.begin();
    while (it != chData.end()) {
        if (it->chPos.x < startX || it->chPos.x > endX ||
            it->chPos.y < startY || it->chPos.y > endY) {
            it = chData.erase(it);
        }
        else {
            ++it;
        }
    }

    for (int x = startX; x <= endX; ++x) {
        for (int y = startY; y <= endY; ++y) {

            bool exists = false;
            for (const auto& c : chData) {
                if (c.chPos.x == x && c.chPos.y == y) {
                    exists = true;
                    break;
                }
            }

            if (!exists) {
                Chunks t = Chunks::SURFACE;

                int neighbours = 0;
                for (const auto& existing : chData) {
                    if (existing.type == Chunks::FOREST) {
                        if (std::abs(existing.chPos.x - x) <= 1 && std::abs(existing.chPos.y - y) <= 1) {
                            neighbours++;
                        }
                    }
                }

                float r = rand(0.f, 100.f);
                float fChance = (neighbours > 0) ? 40.f : 97.f;

                bool isEntrance = false;
                for (const auto& c : chData) if (c.type == Chunks::ENTRANCE) isEntrance = true;
                
                if (chWalked >= 250 && !isEntrance) {
                    float eChance = 99.99f - (std::min((chWalked - 250) * 0.05f, 5.0f));
                    if (r > eChance) t = Chunks::ENTRANCE;
                }

                if (t == Chunks::SURFACE) {
                    if (r > 99.5f) t = Chunks::PIT;
                    else if (r > 98.5f) t = Chunks::CAMP;
                    else if (r > fChance) t = Chunks::FOREST;
                }

                ChunkData data;
                data.type = t;
                data.chPos = { x, y };
                data.showEyes = false;
                data.eyes.setPrimitiveType(sf::PrimitiveType::Lines);
                data.chunk = chunk(t, chSize, sf::Color::Transparent);

                float enRoll = rand(0.f, 100.f);
                if (t == Chunks::CAMP) {
                    for (int i = 0; i < 3; ++i)
                        sEntity(Entities::LURKER, { x * chSize + rand(20,180), y * chSize + rand(20,180) }, { x,y });
                }
                else {
                    if (enRoll > 98.f) {
                        Entities rType = (enRoll > 99.5f) ? Entities::SHINING : Entities::LEECH;
                        sEntity(rType, { x * chSize + 100.f, y * chSize + 100.f }, { x,y });
                    }
                }

                if (t == Chunks::SURFACE && rand(0.f, 100.f) > 85.f) {
                    data.eyePos = { rand(20.f, chSize - 20.f), rand(20.f, chSize - 20.f) };
                    dEyes(data.eyes, data.eyePos);
                }

                chData.push_back(data);
            }
        }
    }

    chBounds.minX = chPos.x * chSize;
    chBounds.minY = chPos.y * chSize;
    chBounds.maxX = (chPos.x + 1) * chSize;
    chBounds.maxY = (chPos.y + 1) * chSize;
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

    for (auto& c : chData) {
        if (curChunk == c.chPos && !c.discovered) {
            c.discovered = true;
            chWalked++;
        }

        if (c.type == Chunks::PIT && !c.isTriggered) {
            if (curChunk == c.chPos) {
                c.isTriggered = true;
                pit(c.chunk, chSize, sf::Color::White);
            }
        }

        else if (c.type == Chunks::ENTRANCE && !c.isTriggered) {
            if (curChunk == c.chPos) {
                c.isTriggered = true;
                playerPos = { 0, 0 };
                chWalked = 0;
                chData.clear();
                currentProg = Ch1Progress::SEQUENCE2;
                return;
            }
        }

        if (c.eyes.getVertexCount() > 0 && !c.showEyes) {
            sf::Vector2f eyeWPos = sf::Vector2f(c.chPos.x * chSize, c.chPos.y * chSize) + c.eyePos;

            float dx = playerPos.x - eyeWPos.x;
            float dy = playerPos.y - eyeWPos.y;
            float dist = std::sqrt(dx * dx + dy * dy);

            if (dist < 500.f) {
                c.showEyes = true;
                for (size_t i = 0; i < c.eyes.getVertexCount(); ++i) {
                    c.eyes[i].color.a = 0;
                }
            }
        }
    }
}