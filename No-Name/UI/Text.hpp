#pragma once
#include "../Misc/Includes.hpp"
#include "../Misc/Variables.hpp"

struct Textbox {
	std::string content;
	float duration = 0.f;
    bool triggered = false;
};

struct CMilestone {
    std::string text;
    float duration;
    bool triggered = false;
};

inline std::queue<Textbox> textQueue;
inline Textbox currentText;
bool isDisplaying = false;

inline std::map<int, CMilestone> caveMilestones = {
    {160, {"Ugh... how much longer?!", 4.0f}},
    {200, {"I'm starting to believe nothing is here...", 5.0f}},
    {220, {"Can't turn back now...", 4.0f}},
    {240, {"Is that... light up ahead?", 5.0f}}
};

inline void checkWalkDistance() {
    for (auto& [distance, milestone] : caveMilestones) {
        if (lWalked >= distance && !milestone.triggered) {
            textQueue.push({ milestone.text, milestone.duration });
            milestone.triggered = true;
        }
    }
}

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