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
inline bool isUltra = false;

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

inline float sanity = 10.f; // Basic psychological horror mechanic, realistically
inline float sanityTimer = 0.f;
inline float sanityGrace = 20.f; // Grace period (seconds) before passive sanity drain begins
inline float hallucinations = 0.f; // Hallucinations ramp up with lower sanity, 1.f max
inline float halluTimer = 0.f;

inline float walkSpd = 0.01f;
inline float bobbing = 0.f;
inline sf::Vector2f bobOffset(0, 0);

inline bool isSprinting = false;
inline float stamina = 100.f;       // 0–100, same convention as battery
inline float staminaDrain = 22.f;   // units per second while sprinting
inline float staminaRegen = 10.f;   // units per second while not sprinting

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
	WRAITH,		// Orbits the player, tightening its spiral. Drains sanity on close approach
				// Counter: Torch flash stuns and pushes it back
	HOLLOW,		// Near-blind, homes on movement only. Stops tracking if player stands still
				// Counter: Stop moving
	STALKER,	// Teleports to vision edge and watches. Drains sanity passively
				// Counter: Keep looking around (looking at it resets its position)
	HUSK,		// Moves in straight lines, bounces off chunk bounds. Spawns in groups
				// Counter: One trap freezes the whole group briefly
	// V-2Y-A
};

inline bool isInitialising = true; // True only during the first rChunks call; suppresses camp spawning
inline float spawnTimer = 0.f;
inline float maxEntitySuspicion = 0.f;

// Chunk variables
enum class Chunks {
	// V-2X-A
	SURFACE,	// Empty chunks, just filler with a chance to spawn items
	PIT,		// A chunk with death pit traps around
	CAMP,		// Spawns 3 entities and a campfire (campfire is just a visual)
	FOREST,		// Slows the player down to 60% speed
	ENTRANCE,	// Escape the surface by going deeper
	MARSH,		// Slows to 40% speed and ticks 1 hp damage over time
	RUIN,		// Collapsed structure, no hazard, visual interest only
	CACHE,		// Guaranteed item spawn, higher-quality rolls
	MIASMA,		// Strange geometry + passive sanity drain, no entities spawn here
	BRAMBLE,	// Slows to 50% and draws entities toward the noise of passing through

	// V-2Y-A
	CORE,		// Empty chunks, just filler with a chance to spawn items
	HOTSPOTS,	// A chunk that would damage the player if stood on
};
inline Chunks genChunks = Chunks::SURFACE;

inline int rDist = 7; // Render distance (how many chunks are rendered from player position)
inline float chSize = 65.f; // Chunk size (in pixels)

inline float marshDmgTimer = 0.f;   // Ticks damage while player is in MARSH
inline float miasmaDrainTimer = 0.f; // Ticks sanity drain while player is in MIASMA

inline sf::Font jetBrainsMono("Assets/Font/JetBrainsMono.ttf");