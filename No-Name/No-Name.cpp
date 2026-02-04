#include "Misc/Audio.hpp"
#include "Misc/Functions.hpp"
#include "Misc/Includes.hpp"
#include "Misc/Variables.hpp"

#include "Chapters/Features/Blink.hpp"
#include "Chapters/Features/Fracture.hpp"
#include "Chapters/Features/Inventory.hpp"
#include "Chapters/Features/Sanity.hpp"

#include "Chapters/Chapter-1/Effects/Bleed.hpp"
#include "Chapters/Chapter-1/Effects/Dust.hpp"
#include "Chapters/Chapter-1/Effects/Hallucinations.hpp"

#include "Chapters/Chapter-1/Sequence-Cave/Cave.hpp"
#include "Chapters/Chapter-1/Sequence-Cave/Shining.hpp"

#include "Chapters/Chapter-1/V-2X-A/Entities.hpp"
#include "Chapters/Chapter-1/V-2X-A/V-2X-A.hpp"

#include "UI/Text.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "No Name", sf::State::Fullscreen);
    window.setFramerateLimit(60);

    initCavern();
    initDust();
    initAudio();
    while (window.isOpen()) {
        float elapsedTime = elapsedClock.getElapsedTime().asSeconds();
        float deltaTime = deltaClock.restart().asSeconds();
        static float lapsed = 0.f;

        window.clear(sf::Color::Black);

        if (currentProg == Ch1Progress::SEQUENCE1) {
            if (!caveDistort) playMusic("cave");
            ambienceRandom();

            uCWalk(bobbing, bobOffset);

            uCavern(elapsedTime, bobOffset, window);
            uDust(elapsedTime, bobOffset, window);

            uBlinks(blink, deltaTime, window);
            uShining(blink, deltaTime, window, bobOffset);

            checkDist();

            if (deadFromShining) {
                currentDiff = Difficulty::EXTREME;
                currentProg = Ch1Progress::V_2X_A;
            }

            if (caveDistort) {
                playMusic("eerie");
                uFracture(blink, deltaTime, window);

                if (crack.isCracking) {
                    playMusic("static");
                    lapsed += deltaTime;

                    if (lapsed >= 5.f) {
                        currentProg = Ch1Progress::V_2X_A;
                        lWalked = 0;
                        lapsed = 0.f;

                        cavern.clear();
                        dust.clear();
                    }
                }
            }
        }

        else if (currentProg == Ch1Progress::V_2X_A) {
            textQueue.push({ "You've entered V_2X_A", 2.f });
            if (lapsed <= 2.f) {
                blind(window);
                lapsed += deltaTime;
            }
        }

		uTextbox(deltaTime, window);

        window.display();

        while (const std::optional gameEvent = window.pollEvent()) {
            if (gameEvent->is<sf::Event::Closed>()) window.close();
        }
    }
}