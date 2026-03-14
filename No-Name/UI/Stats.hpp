#pragma once
#include "../Misc/Includes.hpp"
#include "../Misc/Variables.hpp"

float statsOffset = -220.f;
float statsX = 24.f;

inline void dBar(sf::RenderWindow& window, sf::Vector2f pos,
                 float value, float maxVal,
                 sf::Color barCol, sf::Color bgCol,
                 const std::string& label, float alpha) {

    const float barW    = 120.f;
    const float barH    = 6.f;
    const float labelGap = 8.f;

    sf::Color aBarCol  = barCol; aBarCol.a  = static_cast<uint8_t>(alpha);
    sf::Color aBgCol   = bgCol;  aBgCol.a   = static_cast<uint8_t>(alpha * 0.35f);
    sf::Color aTextCol = sf::Color(200, 200, 200, static_cast<uint8_t>(alpha));

    sf::Text lbl(jetBrainsMono, label, 13);
    lbl.setFillColor(aTextCol);
    lbl.setPosition(pos);
    window.draw(lbl);

    float barY = pos.y + lbl.getGlobalBounds().size.y + labelGap;

    sf::RectangleShape bg({ barW, barH });
    bg.setPosition({ pos.x, barY });
    bg.setFillColor(aBgCol);
    window.draw(bg);

    float fill = std::clamp(value / maxVal, 0.f, 1.f) * barW;
    if (fill > 0.f) {
        sf::RectangleShape bar({ fill, barH });
        bar.setPosition({ pos.x, barY });
        bar.setFillColor(aBarCol);
        window.draw(bar);
    }
}

inline void dRow(sf::RenderWindow& window, sf::Vector2f pos,
                 const std::string& text, float alpha,
                 sf::Color col = sf::Color::White) {
    col.a = static_cast<uint8_t>(alpha);
    sf::Text t(jetBrainsMono, text, 13);
    t.setFillColor(col);
    t.setPosition(pos);
    window.draw(t);
}

inline void uStats(float dt) {
    static bool statsPressed = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Tab)) {
        if (!statsPressed) { statsOpen = !statsOpen; statsPressed = true; }
    }
    else statsPressed = false;

    float slideSpeed = 800.f;
    if (statsOpen) {
        if (statsOffset < statsX) statsOffset += slideSpeed * dt;
        if (statsOffset > statsX) statsOffset  = statsX;
    }
    else {
        if (statsOffset > -220.f) statsOffset -= slideSpeed * dt;
    }
}

inline void dStats(sf::RenderWindow& window) {
    if (statsOffset <= -215.f) return;

    float alpha = std::clamp((statsOffset + 220.f) / (statsX + 220.f) * 255.f, 0.f, 255.f);

    float winH = static_cast<float>(window.getSize().y);
    float x    = statsOffset;
    float rowH = 32.f;
    float y    = winH - 230.f;

    sf::Color hpCol = (hp >= 4) ? sf::Color(220, 220, 220)
                    : (hp == 3) ? sf::Color(220, 200, 80)
                    : (hp == 2) ? sf::Color(220, 130, 60)
                    :             sf::Color(210, 60,  60);
    dBar(window, { x, y }, static_cast<float>(hp), 5.f, hpCol, sf::Color(80, 80, 80), "HP", alpha);
    y += rowH;

    sf::Color sanCol = (sanity >= 7) ? sf::Color(180, 180, 220)
                     : (sanity >= 4) ? sf::Color(160, 120, 200)
                     :                 sf::Color(120, 60,  180);
    dBar(window, { x, y }, static_cast<float>(sanity), 10.f, sanCol, sf::Color(40, 40, 60), "SANITY", alpha);
    y += rowH;

    sf::Color staCol = isSprinting ? sf::Color(180, 220, 180) : sf::Color(120, 160, 120);
    dBar(window, { x, y }, stamina, 100.f, staCol, sf::Color(30, 50, 30), "STAMINA", alpha);
    y += rowH;

    sf::Color batCol = (battery > 40.f) ? sf::Color(220, 210, 130)
                     : (battery > 15.f) ? sf::Color(210, 150, 60)
                     :                    sf::Color(180, 60,  60);
    dBar(window, { x, y }, battery, 100.f, batCol, sf::Color(50, 45, 20),
         torchOn ? "BATTERY [ON]" : "BATTERY", alpha);
    y += rowH + 6.f;

    // SHIELD — text row, only shown when held and not on ULTRA
    if (shield > 0 && currentDiff != Difficulty::ULTRA) {
        dRow(window, { x, y }, "SHIELD  [ACTIVE]", alpha, sf::Color(180, 200, 220));
        y += 22.f;
    }

    dRow(window, { x, y }, "DIST  " + std::to_string(lWalked), alpha, sf::Color(120, 120, 120));
}