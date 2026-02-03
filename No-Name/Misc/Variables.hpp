#pragma once
#include "Includes.hpp"

inline float pi = 3.14159f;

// Mode structure
enum class Difficulty {
	HARD,
	EXTREME,
	ULTRA,
	BRUTAL
};
inline Difficulty currentDiff = Difficulty::HARD;

// Cave shenanigans
inline int l_detail = 60;
inline int mLayers = 15;
inline int lWalked = 0;

inline bool shining = false; // Cave entity designated 'Shining' (tutorial purposes)
inline bool deadFromShining = false;
inline float shiningGrace = 10.f;
inline float shiningDepth = 10.f;

inline bool caveDistort = false;

inline float dustDrift = 0.005f;

// Player variables
inline int hp = 5;
inline int sanity = 10;

inline float walkSpd = 0.01f;
inline float bobbing = 0.f;

inline bool isWalking = false;
inline bool isBlinking = false;

inline bool isDead = false;
inline bool isBleeding = false;

inline sf::Font jetBrainsMono("Assets/Font/JetBrainsMono.ttf");