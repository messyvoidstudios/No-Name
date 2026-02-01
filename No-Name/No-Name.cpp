#include "Misc/Includes.hpp"

#include "Chapter-1/Sequence-Cave/Cave.hpp"


int main() {
    sf::RenderWindow window(sf::VideoMode({ 1920, 1080 }), "No Name", sf::State::Fullscreen);
    window.setFramerateLimit(60);

    sf::Vector2f centre = { window.getSize().x / 2.f, window.getSize().y / 2.f };
    
    std::deque<CaveLayer> cavern;

    int mLayers = 15;
    float spd = 0.05f;

    for (int i = 0; i < mLayers; ++i) {
        cavern.push_back({ layer(1600.f, 900.f, sf::Color::White), (float)i });
    }

    while (window.isOpen()) {
        if (cavern.front().depth <= -1.0f) {
            cavern.pop_front();
            float fDepth = cavern.back().depth + 1.0f;
            cavern.push_back({ layer(1600.f, 900.f, sf::Color::White), fDepth });
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            for (auto& layer : cavern) {
                layer.depth -= spd;
            }
        }

        window.clear(sf::Color::Black);

        for (int i = (int)cavern.size() - 1; i >= 0; --i) {
            auto& layer = cavern[i];

            float scale = std::pow(0.75f, layer.depth);
            uint8_t brightness = static_cast<uint8_t>(255 * std::pow(0.7f, layer.depth));

            for (size_t v = 0; v < layer.vArr.getVertexCount(); ++v) {
                layer.vArr[v].color = sf::Color(brightness, brightness, brightness);
            }

            sf::Transform tx;
            tx.translate(centre);
            tx.scale({ scale, scale });
            tx.translate({ layer.depth * 10.f, layer.depth * 5.f });

            window.draw(layer.vArr, tx);
        }

        window.display();

        while (const std::optional gameEvent = window.pollEvent()) {
            if (gameEvent->is<sf::Event::Closed>()) window.close();
        }
    }
}