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
	BRUTAL,
	ULTRA
};
inline Difficulty currentDiff = Difficulty::HARD;
inline bool isBrutal = false;

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

inline bool shining = false; // Cave entity designated 'Shining' (tutorial purposes)
inline bool deadFromShining = false;
inline float shiningGrace = 10.f; // Time you have to blink
inline float shiningDepth = 10.f; // How far the 'Shining' is from you

inline bool caveDistort = false;

inline float dustDrift = 0.005f;

// Player variables
inline int hp = 5; // Hitpoints, obviously
inline int shield = 0; // Shield, max 1 (essentially +1 hp); ignored for Brutal

inline bool torchOn = false; // Torch toggle (obviously)
inline float battery = 100.f; // Torch battery (percentage)

inline int sanity = 10; // Basic psychological horror mechanic, realistically
inline float sanityTimer = 0.f;
inline float hallucinations = 0.f; // Hallucinations ramp up with lower sanity, 1.f max
inline float halluTimer = 0.f;

inline float walkSpd = 0.01f;
inline float bobbing = 0.f;
inline sf::Vector2f bobOffset(0, 0);

inline int lWalked = 0; // Distance player has walked

inline bool isWalking = false; // Specifically for the Cave Sequence
inline bool isBlinking = false; // If the player is blinking (used for reactive entities)

inline bool isDead = false; // if (hp <= 0) isDead = true;
inline bool isParanoid = false; // If paranoia is inflicted, sanity drains quicker and hallucinations are more common

inline bool invOpen = false;
inline bool statsOpen = false;

// Inventory variables
enum class ItemType { BATTERY, SHIELD, PILLS, TRAP };

inline int activeSlot = 0;

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

inline float spawnTimer = 0.f;

// Chunk variables
enum class Chunks {
	// V-2X-A
	SURFACE,	// Empty chunks, just filler with a chance to spawn items
	PIT,		// A chunk with death pit traps around
	CAMP,		// Spawns 3 entities and a campfire (campfire is just a visual)
	FOREST,		// Slows the player down to 60% speed
	ENTRANCE,	// Escape the surface by going deeper
	// V-2Y-A
	CORE,		// Empty chunks, just filler with a chance to spawn items
	HOTSPOTS,	// A chunk that would damage the player if stood on

};
inline Chunks genChunks = Chunks::SURFACE;

inline int rDist = 7; // Render distance (how many chunks are rendered from player position)
inline float chSize = 65.f; // Chunk size (in pixels)

inline sf::Font jetBrainsMono("Assets/Font/JetBrainsMono.ttf");