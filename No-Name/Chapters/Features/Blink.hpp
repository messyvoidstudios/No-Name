#pragma once
#include "../../Misc/Includes.hpp"

struct Blinking {
    float progress = 0.f;
    bool closing = true;
    bool active = false;
    float spd = 5.0f;
    float timer = 0.f;
    const float interval = 30.f;
};

inline Blinking blink;

inline void uBlinks(Blinking& state, float dt, sf::RenderWindow& window) {
    state.timer += dt;
    if (state.timer >= state.interval) {
        state.active = true;
        state.timer = 0.f;
    }

    bool blinked = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
    if (blinked) {
        state.active = true;
    }

    if (!state.active && !blinked) return;

    if (state.closing) {
        state.progress += state.spd * dt;
        if (state.progress >= 1.0f) {
            state.progress = 1.0f;
            if (!blinked) {
                state.closing = false;
            }
        }
    }
    else {
        state.progress -= state.spd * dt;
        if (state.progress <= 0.0f) {
            state.progress = 0.0f;
            state.closing = true;
            state.active = false;
        }
    }

    float winX = window.getSize().x;
    float winY = window.getSize().y;
    float lidY = (winY / 2.f) * state.progress;

    if (state.progress > 0.f) {
        sf::RectangleShape top({ winX, lidY });
        top.setFillColor(sf::Color::Black);

        sf::RectangleShape bottom({ winX, lidY });
        bottom.setFillColor(sf::Color::Black);
        bottom.setPosition({ 0.f, winY - lidY });

        window.draw(top);
        window.draw(bottom);
    }
}