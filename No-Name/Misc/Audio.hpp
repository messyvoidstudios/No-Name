#pragma once
#include "Includes.hpp"
#include "Variables.hpp"

inline std::map<std::string, std::unique_ptr<sf::Music>> music;
inline std::map<std::string, std::unique_ptr<sf::Music>> ambience;

float bVolMusic = 30.f;

inline void initAudio() {
    std::vector<std::pair<std::string, std::string>> musicToLoad = {
        {"cave", "Assets/Audio/Music/cave.ogg"},
        {"eerie", "Assets/Audio/Music/eerie.ogg"},
        {"static", "Assets/Audio/Ambience/white_noise.ogg"}
    };

    std::vector<std::pair<std::string, std::string>> ambienceToLoad = {
        {"heartbeat", "Assets/Audio/Ambience/heartbeat.ogg"},
        {"screams", "Assets/Audio/Ambience/screams.ogg"}
    };

    for (const auto& [name, path] : musicToLoad) {
        auto track = std::make_unique<sf::Music>();
        if (track->openFromFile(path)) {
            track->setLooping(true);
            track->setVolume(bVolMusic);
            music[name] = std::move(track);
        }
    }

    for (const auto& [name, path] : ambienceToLoad) {
        auto track = std::make_unique<sf::Music>();
        if (track->openFromFile(path)) {
            track->setVolume(bVolMusic);
            ambience[name] = std::move(track);
        }
    }
}

inline void ambienceRandom() {
    if (ambienceClock.getElapsedTime().asSeconds() >= 60.f) {
        if (!ambience.empty()) {
            static std::mt19937 rand(std::random_device{}());
            std::uniform_int_distribution<size_t> dist(0, ambience.size() - 1);

            auto it = ambience.begin();
            std::advance(it, dist(rand));

            it->second->play();
        }
        ambienceClock.restart();
    }
}

inline void playMusic(const std::string& name) {
    if (music.count(name)) {
        if (music[name]->getStatus() == sf::SoundSource::Status::Playing) {
            return;
        }

        for (auto& [key, track] : music) {
            track->stop();
        }

        music[name]->play();
    }
}