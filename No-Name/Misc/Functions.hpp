#pragma once
#include "Includes.hpp"

inline float rand(float min, float max) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

inline void blind(sf::RenderWindow& window) {
    sf::RectangleShape black({ (float)window.getSize().x, (float)window.getSize().y });
    black.setFillColor(sf::Color::Black);
    black.setOrigin({ black.getLocalBounds().size.x / 2.f, black.getLocalBounds().size.y / 2.f });
    black.setPosition({ window.getSize().x / 2.f, window.getSize().y / 2.f });
    window.draw(black);
}