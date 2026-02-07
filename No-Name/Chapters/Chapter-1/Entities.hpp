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

	int sDrain = 1; // If entity is visible, drains 1 sanity every 5 seconds until out of sight
	int sDrainP = 0; // Sanity Drain Plus, certain entities will drain more sanity in the same time period if visible
	int damage = 2; // Base damage that can be dealt by entities
	int damageP = 0; // Damage Plus, scales on Difficulty, starts at +0 (Hard) and is maxed at +3 (Brutal)

    float suspicion = 0.f;

    bool isHallucination = false; // Begins to appear based on `hallucinations`
	bool delEntity = false;

};

inline std::vector<EntityData> enData;

inline sf::VertexArray entity(Entities type, sf::Color c) {
    sf::VertexArray va(sf::PrimitiveType::Lines);

    if (type == Entities::LURKER) {
        float h = 60.f; float w = 10.f;

        va.append(sf::Vertex({ { 0, 0 }, c })); va.append(sf::Vertex({{ 0, -h }, c }));

        va.append(sf::Vertex({ { 0, -h * 0.8f }, c })); va.append(sf::Vertex({{ - w * 2, -h * 0.5f}, c }));
        va.append(sf::Vertex({ { 0, -h * 0.8f }, c })); va.append(sf::Vertex({{ w * 2, -h * 0.5f }, c }));
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
            float a1 = (i / (float)segments) * pi;
            float a2 = ((i + 1) / (float)segments) * pi;
            va.append(sf::Vertex({ { cos(a1) * sSize, sin(a1) * (sSize * 0.5f) }, c }));
            va.append(sf::Vertex({ { cos(a2) * sSize, sin(a2) * (sSize * 0.5f) }, c }));
        }
    }
    return va;
}

void sEntity(Entities type, sf::Vector2f pos, sf::Vector2i chPos) {
    EntityData data;
    data.type = type;
    data.enPos = pos;
    data.enChunk = chPos;
    data.entity = entity(type, sf::Color::White);
    enData.push_back(data);
}

inline void uPatrol(EntityData& en, float dt) {
    float patDist = std::sqrt(std::pow(en.patPos.x - en.enPos.x, 2) + std::pow(en.patPos.y - en.enPos.y, 2));

    if (patDist < 10.f || en.patPos == sf::Vector2f(0, 0)) {
        int offX = (rand() % 3) - 1;
        int offY = (rand() % 3) - 1;

        sf::Vector2i targetChunk = en.enChunk + sf::Vector2i(offX, offY);
        en.patPos = sf::Vector2f(targetChunk.x * chSize + rand(50, 150), targetChunk.y * chSize + rand(50, 150));
    }

    if (en.suspicion < 1.0f) {
        sf::Vector2f diff = en.patPos - en.enPos;
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        en.enPos += (diff / dist) * 30.f * dt;
    }
}