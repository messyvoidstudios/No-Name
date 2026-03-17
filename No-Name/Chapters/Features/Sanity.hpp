#pragma once
#include "../../Misc/Variables.hpp"

inline void uSanity(float dt) {
    if (sanity > 10) sanity = 10;
    if (sanity < 0) sanity = 0;

    if (sanityGrace > 0.f) {
        sanityGrace -= dt;
    } else {
        float drainMultiplier = isParanoid ? 2.0f : 1.0f;
        sanityTimer += dt * drainMultiplier;

        if (sanityTimer >= 60.f) {
            if (sanity > 0) sanity -= 1.f;
            sanityTimer = 0.f;
        }
    }

    float hallucinationsVal = (10.f - sanity) / 10.f;

    if (hallucinations < hallucinationsVal) hallucinations += 0.05f * dt;
    else if (hallucinations > hallucinationsVal) hallucinations -= 0.05f * dt;
}