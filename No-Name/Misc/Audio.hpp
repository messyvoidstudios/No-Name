#pragma once
#include "Includes.hpp"

inline std::map<std::string, std::unique_ptr<sf::Music>> music;

float bVolMusic = 30.f;

inline void initAudio() {
    std::vector<std::pair<std::string, std::string>> musicToLoad = {
        {"cave", "Assets/Audio/Music/cave.ogg"}
    };

    for (const auto& [name, path] : musicToLoad) {
        auto track = std::make_unique<sf::Music>();
        if (track->openFromFile(path)) {
            track->setLooping(true);
            track->setVolume(bVolMusic);
            music[name] = std::move(track);
        }
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