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
inline Ch1Progress currentProg = Ch1Progress::V_2X_A;

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
inline int hp = 5; // Hitpoints, obviously
inline int shield = 0; // Shield, max 1 (essentially +1 hp); ignored for Brutal
inline int sanity = 10; // Basic psychological horror mechanic, realistically
inline float hallucinations = 0.f; // Hallucinations ramp up with lower sanity, 1.f max

inline float walkSpd = 0.01f;
inline float bobbing = 0.f;
inline sf::Vector2f bobOffset(0, 0);

inline bool isWalking = false; // Specifically for the Cave Sequence
inline bool isBlinking = false; // If the player is blinking (used for reactive entities)

inline bool isDead = false; // if (hp <= 0) isDead = true;
inline bool isBleeding = false;
inline bool isParanoid = false; // If paranoia is inflicted, sanity drains quicker and hallucinations are more common

// Entity variables
enum class Entities {
	// V-2X-A
	LURKER,		// Inflicts paranoia, leading to quicker sanity drain
				// Counter: Face its direction
	LEECH,		// Annoying entity that will steal random items from your inventory
				// Counter: None, good luck lmao
	SHINING,	// Did you really think it was only for the tutorial?
				// Counter: As the tutorial taught you, it's better if you don't see
	// V-2Y-A
};
inline Entities selEntity;

inline int sDrain = 1; // If entity is visible, drains 1 sanity every 5 seconds until out of sight
inline int sDrainP = 0; // Sanity Drain Plus, certain entities will drain more sanity in the same time period if visible
inline int damage = 2; // Base damage that can be dealt by entities
inline int damageP = 0; // Damage Plus, scales on Difficulty, starts at +0 (Hard) and is maxed at +3 (Brutal)

inline bool isHallucination; // Begins to appear based on `hallucinations`

// Chunk variables
enum class Chunks {
	// V-2X-A
	BASIC,		// Empty chunks, just filler with a chance to spawn items
	PIT,		// A chunk with death pit traps around
	CAMP,		// Probably turn around, high entity spawnrates but an influx of items
	FOREST,		// Higher Lurker spawnrates
	ENTRANCE,	// Escape the surface by going deeper
	// V-2Y-A
};
inline Chunks genChunks = Chunks::BASIC;

inline int rDist = 7; // Render distance (how many chunks are rendered from player position)
inline float chSize = 65.f; // Chunk size (in pixels)

inline sf::Font jetBrainsMono("Assets/Font/JetBrainsMono.ttf");