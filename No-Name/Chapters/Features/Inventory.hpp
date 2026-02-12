#pragma once
#include "../../Misc/Includes.hpp"
#include "../../Misc/Variables.hpp"

struct Item {
    std::string name;
    ItemType type;
    int count = 1;
    int maxStack = 8;
    bool canStack = true;
    sf::Color col = sf::Color::White;
};

struct ItemWorld {
    ItemType type;
    sf::Vector2f pos;
    bool pickedUp = false;
};

struct PlacedTrap {
    sf::Vector2f pos;
    bool sprung = false;
    float lifetime = 30.f;
};

inline std::vector<Item> inventory;

inline std::vector<ItemWorld> worldItems;

inline std::vector<PlacedTrap> activeTraps;

float invOffset = 150.f;
float invY = 0.f;

inline sf::VertexArray getItemShape(ItemType type, sf::Vector2f pos, sf::Color col, float scale = 1.0f) {
    sf::VertexArray va(sf::PrimitiveType::Lines);
    if (type == ItemType::BATTERY) {
        float w = 10.f * scale, h = 15.f * scale;
        va.append({ {pos.x - w, pos.y - h}, col }); va.append({ {pos.x + w, pos.y - h}, col });
        va.append({ {pos.x + w, pos.y - h}, col }); va.append({ {pos.x + w, pos.y + h}, col });
        va.append({ {pos.x + w, pos.y + h}, col }); va.append({ {pos.x - w, pos.y + h}, col });
        va.append({ {pos.x - w, pos.y + h}, col }); va.append({ {pos.x - w, pos.y - h}, col });
    }
    else if (type == ItemType::SHIELD) {
        for (int i = 0; i < 8; ++i) {
            float a1 = (i / 8.f) * 6.28f, a2 = ((i + 1) / 8.f) * 6.28f;
            va.append({ {pos.x + std::cos(a1) * 12 * scale, pos.y + std::sin(a1) * 12 * scale}, col });
            va.append({ {pos.x + std::cos(a2) * 12 * scale, pos.y + std::sin(a2) * 12 * scale}, col });
        }
        va.append({ {pos.x - 2, pos.y}, col }); va.append({ {pos.x + 2, pos.y}, col });
    }
    else if (type == ItemType::PILLS) {
        float r = 8.f * scale;
        va.append({ {pos.x - r, pos.y - r}, col }); va.append({ {pos.x + r, pos.y - r}, col });
        va.append({ {pos.x + r, pos.y + r}, col }); va.append({ {pos.x - r, pos.y + r}, col });
    }
    else if (type == ItemType::TRAP) {
        va.append({ {pos.x - 10, pos.y - 10}, col }); va.append({ {pos.x + 10, pos.y + 10}, col });
        va.append({ {pos.x + 10, pos.y - 10}, col }); va.append({ {pos.x - 10, pos.y + 10}, col });
    }
    return va;
}

inline void addItem(ItemType type, std::string name, sf::Color col, bool stackable) {
    if (stackable) {
        for (auto& slot : inventory) {
            if (slot.type == type && slot.count < slot.maxStack) {
                slot.count++;
                return;
            }
        }
    }

    if (inventory.size() < 4) {
        inventory.push_back({ name, type, 1, 8, stackable, col });
    }
}
inline void stealItem() {
    if (inventory.empty()) return;
    int index = std::rand() % inventory.size();
    inventory.erase(inventory.begin() + index);
}

inline void consumeShield() {
    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if (it->type == ItemType::SHIELD) {
            inventory.erase(it);
            shield = 0;
            return;
        }
    }
}

inline void uPickups(sf::Vector2f playerPos) {
    auto it = worldItems.begin();
    while (it != worldItems.end()) {
        sf::Vector2f diff = it->pos - playerPos;
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

        if (dist < 20.f) {
            bool stack = (it->type == ItemType::BATTERY || it->type == ItemType::PILLS);
            std::string name = (it->type == ItemType::BATTERY) ? "Battery" :
                (it->type == ItemType::SHIELD) ? "Shield" :
                (it->type == ItemType::PILLS) ? "Pills" : "Trap";

            sf::Color col = (it->type == ItemType::BATTERY) ? sf::Color(243, 238, 225) :
                (it->type == ItemType::SHIELD) ? sf::Color(243, 238, 225) :
                (it->type == ItemType::PILLS) ? sf::Color(243, 238, 225) : sf::Color(243, 238, 225);

            if (inventory.size() < 4 || stack) {
                addItem(it->type, name, col, stack);
                it = worldItems.erase(it);
                continue;
            }
        }
        ++it;
    }
}

inline void uInventory(float dt) {
    static bool invPressed = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I)) {
        if (!invPressed) {
            invOpen = !invOpen;
            invPressed = true;
        }
    }
    else invPressed = false;

    float slideSpeed = 800.f;
    if (invOpen) {
        if (invOffset > invY) invOffset -= slideSpeed * dt;
        if (invOffset < invY) invOffset = invY;
    }
    else {
        if (invOffset < 150.f) invOffset += slideSpeed * dt;
    }
}

inline void dInventory(sf::RenderWindow& window) {
    if (invOffset >= 145.f && !invOpen) return;

    float slotSize = 60.f;
    float padding = 10.f;
    float totalW = (slotSize * 4) + (padding * 3);
    sf::Vector2f startPos(window.getSize().x / 2.f - totalW / 2.f, (window.getSize().y - 100.f) + invOffset);

    for (int i = 0; i < 4; ++i) {
        sf::RectangleShape slot({ slotSize, slotSize });
        slot.setPosition({ startPos.x + (i * (slotSize + padding)), startPos.y });

        float alpha = std::clamp(255.f * (1.0f - (invOffset / 150.f)), 0.f, 255.f);

        slot.setFillColor(sf::Color(5, 5, 5, static_cast<uint8_t>(alpha)));

        if (i == activeSlot) {
            slot.setOutlineColor(sf::Color(243, 238, 225, static_cast<uint8_t>(alpha)));
            slot.setOutlineThickness(3.f);
        }
        else {
            slot.setOutlineColor(sf::Color(30, 30, 30, static_cast<uint8_t>(alpha)));
            slot.setOutlineThickness(2.f);
        }

        window.draw(slot);

        if (i < inventory.size()) {
            sf::Color itemCol = inventory[i].col;
            itemCol.a = static_cast<uint8_t>(alpha);

            sf::VertexArray va = getItemShape(inventory[i].type,
                { slot.getPosition().x + 30.f, slot.getPosition().y + 30.f },
                itemCol, 1.5f);
            window.draw(va);
        }
    }
}