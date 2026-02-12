#pragma once
#include "../../Misc/Variables.hpp"

inline void uSanity(float dt) {
    if (sanity > 10) sanity = 10;
    if (sanity < 0) sanity = 0;

    float drainMultiplier = isParanoid ? 2.0f : 1.0f;
    sanityTimer += dt * drainMultiplier;

    if (sanityTimer >= 60.f) {
        if (sanity > 0) sanity -= 1;
        sanityTimer = 0.f;
    }

    float hallucinationsVal = (10.f - static_cast<float>(sanity)) / 10.f;

    if (hallucinations < hallucinationsVal) hallucinations += 0.05f * dt;
    else if (hallucinations > hallucinationsVal) hallucinations -= 0.05f * dt;
}