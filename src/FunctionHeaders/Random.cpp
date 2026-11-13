#include "Random.hpp"

#include <random>

#include <SDL3/SDL_stdinc.h>

namespace Random::etc {
    thread_local Uint64 _Random_Seed =
        static_cast<Uint64>(std::random_device()());
}

// Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs"
Uint64 Random::etc::xorshift() noexcept {
    etc::_Random_Seed ^= etc::_Random_Seed << 13;
    etc::_Random_Seed ^= etc::_Random_Seed >> 7;
    etc::_Random_Seed ^= etc::_Random_Seed << 17;
    return etc::_Random_Seed;
}