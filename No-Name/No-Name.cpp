#include "Misc/Includes.hpp"
#include "Misc/Functions.hpp"
#include "Misc/Variables.hpp"

#include "Chapter-1/Effects/Dust.hpp"

#include "Chapter-1/Sequence-Cave/Cave.hpp"


int main() {
    sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "No Name", sf::State::Fullscreen);
    window.setFramerateLimit(60);

    sf::Vector2f centre = { window.getSize().x / 2.f, window.getSize().y / 2.f };

    sf::Clock elapsedClock;
    
    std::deque<CLayer> cavern;
	std::vector<Dust> dust;

    for (int i = 0; i < mLayers; ++i) {
        cavern.push_back({ layer(1600.f, 900.f, sf::Color::White), (float)i });
    }

    for (int i = 0; i < 100; ++i) {
        dust.push_back({ {rand(-800, 800), rand(-500, 500)}, rand(0, 10), rand(1.2f, 2.0f), rand(0, 6.28f) });
    }

    while (window.isOpen()) {
        float elapsedTime = elapsedClock.getElapsedTime().asSeconds();

        sf::Vector2f bobOffset(0, 0);

        bool isWalking = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);

        if (isWalking) {
            for (auto& layer : cavern) layer.depth -= walkSpd;
            bobbing += 0.12f;
            bobOffset.x = std::cos(bobbing * 0.5f) * 12.f;
            bobOffset.y = std::sin(bobbing) * 8.f;
        }

        for (auto& d : dust) {
            float currentSpeed = isWalking ? (walkSpd * d.spdMult) : dustDrift;
            d.depth -= currentSpeed;

            if (d.depth < 0) {
                d.depth = 10.f;
                d.pos = { rand(-1000, 1000), rand(-600, 600) };
            }
        }

        if (cavern.front().depth <= -1.0f) {
            cavern.pop_front();
            cavern.push_back({ layer(1600.f, 900.f, sf::Color::White), cavern.back().depth + 1.0f });
        }

        window.clear(sf::Color::Black);

        for (int i = (int)cavern.size() - 1; i >= 0; --i) {
            auto& layer = cavern[i];

            float scale = std::pow(0.75f, std::max(0.01f, layer.depth));
            float lightFactor = std::pow(0.7f, std::max(0.01f, layer.depth));

            float alphaFactor = 1.0f;
            if (layer.depth < 1.5f) {
                alphaFactor = std::clamp((layer.depth - 0.2f) / 1.3f, 0.0f, 1.0f);
            }
            else if (layer.depth > (mLayers - 3)) {
                alphaFactor = std::clamp((mLayers - layer.depth) / 3.0f, 0.0f, 1.0f);
            }

            uint8_t rgb = static_cast<uint8_t>(255 * lightFactor);
            uint8_t a = static_cast<uint8_t>(255 * alphaFactor);

            for (size_t v = 0; v < layer.vArr.getVertexCount(); ++v) {
                layer.vArr[v].color = sf::Color(rgb, rgb, rgb, a);
            }

            sf::Transform tx;
            tx.translate(centre + bobOffset);
            tx.scale({ scale, scale });
            float drift = std::max(0.f, layer.depth);
            tx.translate({ drift * 10.f, drift * 5.f });

            window.draw(layer.vArr, tx);
        }

        sf::VertexArray dustParticles(sf::PrimitiveType::Points);
        for (const auto& d : dust) {
            float s = std::pow(0.75f, d.depth);

            float sinX = std::sin(elapsedTime + d.offset) * 20.f;
            float cosY = std::cos(elapsedTime * 0.7f + d.offset) * 15.f;

            sf::Vector2f fPos = centre + bobOffset + ((d.pos + sf::Vector2f(sinX, cosY)) * s);

            float a = std::clamp(d.depth / 2.0f, 0.0f, 0.5f);
            dustParticles.append(sf::Vertex({ fPos, sf::Color(200, 200, 200, (uint8_t)(255 * a)) }));
        }
        window.draw(dustParticles);

        window.display();

        while (const std::optional gameEvent = window.pollEvent()) {
            if (gameEvent->is<sf::Event::Closed>()) window.close();
        }
    }
}