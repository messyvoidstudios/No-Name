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

inline std::vector<ChunkData> chData;

inline ChunkBounds chBounds;

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
        float t = static_cast<float>(i) / detail;
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

inline void dReedCluster(sf::VertexArray& ch, sf::Vector2f pos, sf::Color c) {
    for (int i = 0; i < 3; ++i) {
        float ox = pos.x + i * 4.f - 4.f;
        float h  = rand(8.f, 16.f);
        ch.append(sf::Vertex({ { ox,        pos.y      }, c }));
        ch.append(sf::Vertex({ { ox,        pos.y - h  }, c }));
        ch.append(sf::Vertex({ { ox,        pos.y - h  }, c }));
        ch.append(sf::Vertex({ { ox - 3.f,  pos.y - h - 4.f }, c }));
        ch.append(sf::Vertex({ { ox,        pos.y - h  }, c }));
        ch.append(sf::Vertex({ { ox + 3.f,  pos.y - h - 4.f }, c }));
    }
}

inline void dRuinWall(sf::VertexArray& ch, sf::Vector2f pos, float w, float h, sf::Color c) {
    ch.append(sf::Vertex({ { pos.x,     pos.y     }, c }));
    ch.append(sf::Vertex({ { pos.x,     pos.y - h }, c }));
    ch.append(sf::Vertex({ { pos.x,     pos.y     }, c }));
    ch.append(sf::Vertex({ { pos.x + w, pos.y     }, c }));
    ch.append(sf::Vertex({ { pos.x + w, pos.y     }, c }));
    ch.append(sf::Vertex({ { pos.x + w, pos.y - h * 0.7f }, c }));
    float step = w / 4.f;
    float heights[] = { h, h * 0.55f, h * 0.85f, h * 0.4f };
    for (int i = 0; i < 3; ++i) {
        ch.append(sf::Vertex({ { pos.x + step * i,       pos.y - heights[i]   }, c }));
        ch.append(sf::Vertex({ { pos.x + step * (i + 1), pos.y - heights[i+1] }, c }));
    }
}

inline void dMiasmaGlyph(sf::VertexArray& ch, sf::Vector2f pos, sf::Color c) {
    ch.append(sf::Vertex({ { pos.x,        pos.y - 14.f }, c }));
    ch.append(sf::Vertex({ { pos.x + 12.f, pos.y + 8.f  }, c }));
    ch.append(sf::Vertex({ { pos.x + 12.f, pos.y + 8.f  }, c }));
    ch.append(sf::Vertex({ { pos.x - 12.f, pos.y + 8.f  }, c }));
    ch.append(sf::Vertex({ { pos.x - 12.f, pos.y + 8.f  }, c }));
    ch.append(sf::Vertex({ { pos.x,        pos.y - 14.f  }, c }));
    ch.append(sf::Vertex({ { pos.x - 16.f, pos.y - 2.f  }, c }));
    ch.append(sf::Vertex({ { pos.x + 8.f,  pos.y + 14.f }, c }));
}

inline void dBramblePatch(sf::VertexArray& ch, sf::Vector2f pos, sf::Color c) {
    int spurs = 6 + (std::rand() % 4);
    for (int i = 0; i < spurs; ++i) {
        float angle = (i / static_cast<float>(spurs)) * 2.f * pi + rand(-0.3f, 0.3f);
        float len   = rand(5.f, 12.f);
        sf::Vector2f tip = pos + sf::Vector2f(std::cos(angle) * len, std::sin(angle) * len);
        ch.append(sf::Vertex({ pos, c }));
        ch.append(sf::Vertex({ tip, c }));
        float bAngle = angle + rand(0.4f, 0.9f);
        sf::Vector2f barb = tip + sf::Vector2f(std::cos(bAngle) * 4.f, std::sin(bAngle) * 4.f);
        ch.append(sf::Vertex({ tip,  c }));
        ch.append(sf::Vertex({ barb, c }));
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
        int layer = 6; int segments = 12; float bRad = 55.f;

        for (int i = 0; i < layer; ++i) {
            float colFac = 1.f - (static_cast<float>(i) / layer);
            uint8_t col = static_cast<uint8_t>(30 + (225 * colFac));
            uint8_t a   = static_cast<uint8_t>(50 + (205 * colFac));
            sf::Color layerC(col, col, col, a);
            float rad = bRad * (1.f - (i * 0.14f));
            sf::Vector2f origin;
            for (int j = 0; j <= segments; ++j) {
                float angle = pi + (static_cast<float>(j) / segments) * pi;
                sf::Vector2f pt(mid + std::cos(angle) * rad, groundY + std::sin(angle) * rad);
                if (j > 0 && j < segments) { pt.x += rand(-2.f, 2.f); pt.y += rand(-2.f, 2.f); }
                if (j > 0) { ch.append(sf::Vertex({ origin, layerC })); ch.append(sf::Vertex({ pt, layerC })); }
                origin = pt;
            }
        }
    }

    else if (type == Chunks::MARSH) {
        sf::Color reedC(25, 30, 25);
        sf::Color waterC(15, 20, 25);
        int clusters = 6 + (std::rand() % 6);
        for (int i = 0; i < clusters; ++i)
            dReedCluster(ch, { rand(8.f, size - 8.f), rand(8.f, size - 8.f) }, reedC);
        for (int i = 0; i < 5; ++i) {
            float wy = rand(5.f, size - 5.f);
            float wx = rand(5.f, size * 0.4f);
            float wl = rand(10.f, 30.f);
            ch.append(sf::Vertex({ { wx,      wy }, waterC }));
            ch.append(sf::Vertex({ { wx + wl, wy }, waterC }));
        }
    }

    else if (type == Chunks::RUIN) {
        sf::Color stoneC(35, 33, 30);
        int walls = 2 + (std::rand() % 2);
        for (int i = 0; i < walls; ++i) {
            dRuinWall(ch,
                { rand(8.f, size - 30.f), rand(20.f, size - 8.f) },
                rand(18.f, 35.f), rand(14.f, 28.f), stoneC);
        }
        for (int i = 0; i < 10; ++i) {
            float rx = rand(5.f, size - 5.f), ry = rand(5.f, size - 5.f);
            float rl = rand(3.f, 9.f),        ra = rand(0.f, pi);
            ch.append(sf::Vertex({ { rx, ry }, stoneC }));
            ch.append(sf::Vertex({ { rx + std::cos(ra) * rl, ry + std::sin(ra) * rl }, stoneC }));
        }
    }

    else if (type == Chunks::CACHE) {
        sf::Color cairnC(50, 48, 44);
        float blockW[] = { 16.f, 12.f, 8.f };
        float blockH[] = {  6.f,  5.f, 4.f };
        float curY = mid + 7.f;
        for (int i = 0; i < 3; ++i) {
            float bx = mid - blockW[i] / 2.f + rand(-2.f, 2.f);
            ch.append(sf::Vertex({ { bx,             curY           }, cairnC }));
            ch.append(sf::Vertex({ { bx + blockW[i], curY           }, cairnC }));
            ch.append(sf::Vertex({ { bx,             curY           }, cairnC }));
            ch.append(sf::Vertex({ { bx,             curY - blockH[i] }, cairnC }));
            ch.append(sf::Vertex({ { bx + blockW[i], curY           }, cairnC }));
            ch.append(sf::Vertex({ { bx + blockW[i], curY - blockH[i] }, cairnC }));
            ch.append(sf::Vertex({ { bx,             curY - blockH[i] }, cairnC }));
            ch.append(sf::Vertex({ { bx + blockW[i], curY - blockH[i] }, cairnC }));
            curY -= blockH[i];
        }
    }

    else if (type == Chunks::MIASMA) {
        sf::Color glyphC(28, 28, 32);
        int glyphs = 3 + (std::rand() % 4);
        for (int i = 0; i < glyphs; ++i)
            dMiasmaGlyph(ch, { rand(12.f, size - 12.f), rand(12.f, size - 12.f) }, glyphC);
        for (int ring = 1; ring <= 3; ++ring) {
            float rad = ring * 10.f;
            int seg = 12;
            for (int j = 0; j < seg; ++j) {
                if (j % 3 == 0) continue;
                float a1 = (j / static_cast<float>(seg)) * 2.f * pi;
                float a2 = ((j + 1) / static_cast<float>(seg)) * 2.f * pi;
                ch.append(sf::Vertex({ { mid + std::cos(a1) * rad, mid + std::sin(a1) * rad }, glyphC }));
                ch.append(sf::Vertex({ { mid + std::cos(a2) * rad, mid + std::sin(a2) * rad }, glyphC }));
            }
        }
    }

    else if (type == Chunks::BRAMBLE) {
        sf::Color thornC(22, 18, 16);
        int patches = 8 + (std::rand() % 6);
        for (int i = 0; i < patches; ++i)
            dBramblePatch(ch, { rand(8.f, size - 8.f), rand(8.f, size - 8.f) }, thornC);
    }

    bool skipFill = (type == Chunks::MARSH  || type == Chunks::RUIN   ||
                     type == Chunks::CACHE  || type == Chunks::MIASMA ||
                     type == Chunks::BRAMBLE);

    int treeMax   = skipFill ? 0 : 3;
    float treeScaleM = 0.7f;
    if (type == Chunks::FOREST) { treeMax = 15; treeScaleM = 1.f; }

    int trees = skipFill ? 0 : (2 + (std::rand() % treeMax));
    for (int i = 0; i < trees; ++i) {
        sf::Vector2f tPos(rand(10.f, size - 10.f), rand(10.f, size - 10.f));
        if (type == Chunks::CAMP     && std::abs(tPos.x - mid) < 60) continue;
        if (type == Chunks::ENTRANCE && std::abs(tPos.x - mid) < 60) continue;
        dTree(ch, tPos, rand(treeScaleM, 1.5f));
    }

    int grassMult   = (type == Chunks::FOREST) ? 3 : 1;
    int grassBlades = skipFill ? 0 : ((15 + (std::rand() % 20)) * grassMult);
    for (int i = 0; i < grassBlades; ++i) {
        sf::Vector2f gPos(rand(5.f, size - 5.f), rand(5.f, size - 5.f));
        if (type == Chunks::ENTRANCE && std::abs(gPos.x - mid) < 60) continue;
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

                int campNeighbours = 0;
                for (const auto& existing : chData) {
                    if (existing.type == Chunks::CAMP) {
                        if (std::abs(existing.chPos.x - x) <= 1 && std::abs(existing.chPos.y - y) <= 1) {
                            campNeighbours++;
                        }
                    }
                }
                float campSpawnBoost = campNeighbours * 0.4f; // Each adjacent camp lowers the threshold by 0.4 (≈20% more spawns)

                float r = rand(0.f, 100.f);
                float fChance = (neighbours > 0) ? 40.f : 97.f;

                bool isEntrance = false;
                for (const auto& c : chData) if (c.type == Chunks::ENTRANCE) isEntrance = true;

                if (lWalked >= 250 && !isEntrance) {
                    float eChance = 99.99f - (std::min((lWalked - 250) * 0.05f, 5.f));
                    if (r > eChance) t = Chunks::ENTRANCE;
                }

                if (t == Chunks::SURFACE) {
                    if      (r > 99.5f) t = Chunks::PIT;
                    else if (!isInitialising && r > 98.5f) t = Chunks::CAMP;
                    else if (r > 98.0f) t = Chunks::CACHE;
                    else if (r > 97.2f) t = Chunks::RUIN;
                    else if (r > 96.0f) t = Chunks::MIASMA;
                    else if (r > 94.5f) t = Chunks::MARSH;
                    else if (r > 93.0f) t = Chunks::BRAMBLE;
                    else if (r > fChance) t = Chunks::FOREST;
                }

                ChunkData data;
                data.type = t;
                data.chPos = { x, y };
                data.showEyes = false;
                data.eyes.setPrimitiveType(sf::PrimitiveType::Lines);
                data.chunk = chunk(t, chSize, sf::Color::Transparent);

                float enRoll = rand(0.f, 100.f);
                float enThreshold = 98.f - campSpawnBoost;

                if (t == Chunks::CAMP) {
                    for (int i = 0; i < 3; ++i)
                        sEntity(Entities::LURKER, { x * chSize + rand(20,180), y * chSize + rand(20,180) }, { x,y });
                }
                else {
                    if (enRoll > enThreshold) {
                        Entities rType;
                        if      (enRoll > (99.5f - campSpawnBoost)) rType = Entities::SHINING;
                        else if (enRoll > (99.0f - campSpawnBoost)) rType = Entities::WRAITH;
                        else if (enRoll > (98.5f - campSpawnBoost)) rType = Entities::HUSK;
                        else                                         rType = Entities::LEECH;

                        sEntity(rType, { x * chSize + 100.f, y * chSize + 100.f }, { x,y });

                        // Husk group: spawn 1-2 extras nearby
                        if (rType == Entities::HUSK) {
                            int extras = 1 + (std::rand() % 2);
                            for (int i = 0; i < extras; ++i) {
                                sEntity(Entities::HUSK,
                                    { x * chSize + rand(20.f, chSize - 20.f), y * chSize + rand(20.f, chSize - 20.f) },
                                    { x, y });
                            }
                        }
                    }
                }

                if (t == Chunks::SURFACE && enRoll > 85.f) {
                    data.eyePos = { rand(20.f, chSize - 20.f), rand(20.f, chSize - 20.f) };
                    dEyes(data.eyes, data.eyePos);
                }

                if (t == Chunks::MIASMA) {
                    data.eyePos = { rand(20.f, chSize - 20.f), rand(20.f, chSize - 20.f) };
                    dEyes(data.eyes, data.eyePos);
                }

                if (t == Chunks::CACHE) {
                    ItemWorld cacheItem;
                    cacheItem.type = static_cast<ItemType>(std::rand() % 3); // BATTERY/SHIELD/PILLS only
                    cacheItem.pos  = { x * chSize + chSize / 2.f, y * chSize + chSize / 2.f };
                    worldItems.push_back(cacheItem);
                }
                else if (r > 98.f) {
                    ItemWorld newItem;
                    newItem.type = static_cast<ItemType>(std::rand() % 4);
                    newItem.pos  = { x * chSize + rand(50.f, 150.f), y * chSize + rand(50.f, 150.f) };
                    worldItems.push_back(newItem);
                }

                chData.push_back(data);
            }
        }
    }

    chBounds.minX = chPos.x * chSize;
    chBounds.minY = chPos.y * chSize;
    chBounds.maxX = (chPos.x + 1) * chSize;
    chBounds.maxY = (chPos.y + 1) * chSize;

    isInitialising = false;
}

inline void pit(sf::VertexArray& cir, float size, sf::Color c) {
    cir.clear();
    int segments = 16;
    float rad = size * 0.4f;
    sf::Vector2f centre(size / 2.f, size / 2.f);

    for (int i = 0; i < segments; ++i) {
        float a1 = (i / static_cast<float>(segments)) * 2.f * pi;
        float a2 = ((i + 1) / static_cast<float>(segments)) * 2.f * pi;

        cir.append(sf::Vertex({ centre + sf::Vector2f(std::cos(a1) * rad, std::sin(a1) * rad), c }));
        cir.append(sf::Vertex({ centre + sf::Vector2f(std::cos(a2) * rad, std::sin(a2) * rad), c }));
    }
}

inline void uChunks(sf::Vector2f playerPos, float dt) {
    sf::Vector2i curChunk(static_cast<int>(std::floor(playerPos.x / chSize)), static_cast<int>(std::floor(playerPos.y / chSize)));

    for (auto& c : chData) {
        if (curChunk == c.chPos && !c.discovered) {
            c.discovered = true;
            lWalked++;
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
                lWalked = 0;
                chData.clear();
                currentProg = Ch1Progress::SEQUENCE2;
                return;
            }
        }

        if (curChunk == c.chPos) {
            if (c.type == Chunks::MARSH) {
                marshDmgTimer += dt;
                if (marshDmgTimer >= 4.f) { hp -= 1; marshDmgTimer = 0.f; }
            } else {
                marshDmgTimer = 0.f;
            }

            if (c.type == Chunks::MIASMA) {
                miasmaDrainTimer += dt;
                if (miasmaDrainTimer >= 3.f) { sanity -= 1; miasmaDrainTimer = 0.f; }
            } else {
                miasmaDrainTimer = 0.f;
            }

            if (c.type == Chunks::BRAMBLE) {
                for (auto& en : enData) {
                    sf::Vector2f d = en.enPos - playerPos;
                    float dist = std::sqrt(d.x * d.x + d.y * d.y);
                    if (dist < chSize * 4.f) en.suspicion += 1.5f * dt;
                }
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