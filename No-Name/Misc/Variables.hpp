#pragma once
#include "Includes.hpp"

inline float pi = 3.14159f;

// Clocks
inline sf::Clock ambienceClock;
inline sf::Clock elapsedClock;
inline sf::Clock deltaClock;

// Mode structure
enum class Difficulty {
	HARD,
	EXTREME,
	ULTRA,
	BRUTAL
};
inline Difficulty currentDiff = Difficulty::HARD;

enum class Ch1Progress {
	SEQUENCE1,
	V_2X_A,
	SEQUENCE2,
	V_2Y_A,
	SEQUENCEF
};
inline Ch1Progress currentProg = Ch1Progress::SEQUENCE1;

// Cave shenanigans
inline int l_detail = 60;
inline int mLayers = 15;
inline int lWalked = 0;

inline bool shining = false; // Cave entity designated 'Shining' (tutorial purposes)
inline bool deadFromShining = false;
inline float shiningGrace = 10.f; // Time you have to blink
inline float shiningDepth = 10.f; // How far the 'Shining' is from you

inline bool caveDistort = false;

inline float dustDrift = 0.005f;

// Player variables
inline int hp = 5;
inline int sanity = 10;

inline float walkSpd = 0.01f;
inline float bobbing = 0.f;
inline sf::Vector2f bobOffset(0, 0);

inline bool isWalking = false; // Specifically for the Cave Sequence
inline bool isBlinking = false;

inline bool isDead = false;
inline bool isBleeding = false;

inline sf::Font jetBrainsMono("Assets/Font/JetBrainsMono.ttf");