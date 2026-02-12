#pragma once
#include "../Misc/Includes.hpp"
#include "../Misc/Variables.hpp"

float statsOffset = -200.f;
float statsX = 20.f;

inline void uStats(float dt) {
	static bool statsPressed = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Tab)) {
        if (!statsPressed) {
			statsOpen = !statsOpen;
            statsPressed = true;
        }
    }
    else statsPressed = false;

    float slideSpeed = 800.f;
    if (statsOpen) {
        if (statsOffset < statsX) statsOffset += slideSpeed * dt;
        if (statsOffset > statsX) statsOffset = statsX;
    }
    else {
        if (statsOffset > -200.f) statsOffset -= slideSpeed * dt;
    }

    shield = 0;
    for (const auto& item : inventory) {
        if (item.type == ItemType::SHIELD) {
            shield = 1;
            break;
        }
    }
}

inline void dStats(sf::RenderWindow& window) {
    if (statsOffset <= -195.f) return;
    
    std::string info =
        "HP: " + std::to_string(hp) + "\n" +
        "SHIELD: " + (currentDiff == Difficulty::ULTRA ? "NONE" : std::to_string(shield)) + "\n" +
        "SANITY: " + std::to_string(sanity) + "/10\n\n" +
        "WALKED: " + std::to_string(lWalked);

    sf::Text text(jetBrainsMono, info, 18);
    text.setFillColor(sf::Color::White);

    float yPos = window.getSize().y - text.getGlobalBounds().size.y - 40.f;
    text.setPosition({ statsOffset, yPos });

    window.draw(text);
}