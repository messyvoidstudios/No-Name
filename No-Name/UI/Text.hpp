#pragma once
#include "../Misc/Includes.hpp"
#include "../Misc/Variables.hpp"

#include "../Chapters/Chapter-1/Sequence-Cave/Cave.hpp"

struct Textbox {
	std::string content;
	float duration = 0.f;
    bool triggered = false;
};

inline std::queue<Textbox> textQueue;
inline Textbox currentText;
inline bool isDisplaying = false;

inline void uTextbox(float dt, sf::RenderWindow& window) {
    if (!isDisplaying && !textQueue.empty()) {
        currentText = textQueue.front();
        textQueue.pop();
        isDisplaying = true;
    }

    if (!isDisplaying) return;

    currentText.duration -= dt;
    if (currentText.duration <= 0.f) {
        isDisplaying = false;
        return;
    }

    sf::RectangleShape bg({ window.getSize().x * 0.7f, 100.f });
    bg.setPosition({ window.getSize().x / 2.f, window.getSize().y - 80.f });
    bg.setOrigin({ bg.getSize().x / 2.f, bg.getSize().y / 2.f });
    bg.setFillColor(sf::Color(10, 10, 10));
    bg.setOutlineThickness(1.f);
    bg.setOutlineColor(sf::Color::White);

    sf::Text t(jetBrainsMono, currentText.content, 20);
    t.setOrigin({ t.getLocalBounds().size.x / 2.f, t.getLocalBounds().size.y / 2.f });
    t.setPosition(bg.getPosition());
    t.setFillColor(sf::Color(243, 238, 225));

    window.draw(bg);
    window.draw(t);
}