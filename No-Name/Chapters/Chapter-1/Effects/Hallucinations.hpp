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

static const std::vector<std::string> halluMessages = {
    "YOU SHOULDN'T BE HERE",
    "GET OUT",
    "LEAVE",
    "IT SEES YOU",
    "DON'T LOOK",
    "TURN BACK",
    "YOU'RE NOT ALONE",
    "IT'S BEHIND YOU",
    "RUN",
    "YOU'RE GOING TO DIE HERE",
    "THEY'RE WATCHING",
    "YOU CAN'T HIDE",
    "GO BACK",
    "THIS PLACE IS WRONG",
    "DON'T BREATHE",
    "YOU'RE ALREADY LOST",
    "IT'S TOO LATE",
    "STAY STILL",
    "DON'T MOVE",
    "STOP",
    "YOU HEARD THAT TOO, RIGHT?",
    "CLOSER",
    "WAKE UP",
    "YOU'RE DREAMING",
};

struct HalluTextEntry {
    std::string str;
    sf::Vector2f pos;
    float lifetime;
    float spasm;
    float shakeX = 0.f;
    float shakeY = 0.f;
    unsigned int charSize;
    float alpha;
};

inline std::vector<HalluTextEntry> halluTexts;
inline float halluSpawnTimer = 0.f;

inline void uHalluText(float dt, sf::RenderWindow& window) {
    if (hallucinations < 0.35f) {
        halluTexts.clear();
        halluSpawnTimer = 0.f;
        return;
    }

    float h = hallucinations;

    float spawnInterval = 12.f - (h * 10.5f);
    halluSpawnTimer -= dt;
    if (halluSpawnTimer <= 0.f) {
        HalluTextEntry e;
        e.str       = halluMessages[std::rand() % halluMessages.size()];
        e.pos       = { rand(80.f,  static_cast<float>(window.getSize().x) - 80.f),
                        rand(60.f,  static_cast<float>(window.getSize().y) - 60.f) };
        e.lifetime  = rand(2.5f, 5.f);
        e.spasm     = rand(0.3f, 1.2f);
        e.charSize  = static_cast<unsigned int>(10 + h * 14.f);
        e.alpha     = 0.f;
        halluTexts.push_back(e);
        halluSpawnTimer = spawnInterval;
    }

    float shakeScale = 2.f + h * 18.f;
    float spasScale  = 0.3f + h * 0.8f;

    auto it = halluTexts.begin();
    while (it != halluTexts.end()) {
        it->lifetime -= dt;
        if (it->lifetime <= 0.f) {
            it = halluTexts.erase(it);
            continue;
        }

        float fade    = std::clamp(it->lifetime / 0.6f, 0.f, 1.f)
                      * std::clamp((5.f - it->lifetime) / 0.4f, 0.f, 1.f);
        float maxAlpha = 80.f + h * 175.f;
        it->alpha = fade * maxAlpha;

        it->shakeX = rand(-shakeScale, shakeScale);
        it->shakeY = rand(-shakeScale, shakeScale);

        it->spasm -= dt;
        if (it->spasm <= 0.f) {
            it->pos   = { rand(80.f,  static_cast<float>(window.getSize().x) - 80.f),
                          rand(60.f,  static_cast<float>(window.getSize().y) - 60.f) };
            it->spasm = spasScale * rand(0.2f, 1.0f);

            it->shakeX = rand(-shakeScale * 2.5f, shakeScale * 2.5f);
            it->shakeY = rand(-shakeScale * 2.5f, shakeScale * 2.5f);
        }

        ++it;
    }
}

inline void dHalluText(sf::RenderWindow& window) {
    if (halluTexts.empty()) return;

    for (const auto& e : halluTexts) {
        if (e.alpha < 1.f) continue;

        sf::Text t(jetBrainsMono, e.str, e.charSize);

        sf::FloatRect bounds = t.getLocalBounds();
        t.setOrigin({ bounds.position.x + bounds.size.x / 2.f,
                      bounds.position.y + bounds.size.y / 2.f });

        t.setPosition({ e.pos.x + e.shakeX, e.pos.y + e.shakeY });

        uint8_t a = static_cast<uint8_t>(std::clamp(e.alpha, 0.f, 255.f));
        t.setFillColor(sf::Color(243, 238, 225, a));

        window.draw(t);
    }
}