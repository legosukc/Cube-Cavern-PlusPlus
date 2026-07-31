#include "Statistics.hpp"

#include <SDL3/SDL_stdinc.h>

namespace Game::Statistics {

    namespace Memory {

        volatile Uint64 EngineAllocationsPerFrame = 0;
        volatile Uint64 LuaAllocationsPerFrame = 0;

        volatile Uint64 EngineAllocationBytesPerFrame = 0;
        volatile Uint64 LuaAllocationsBytesPerFrame = 0;

        volatile Uint64 EngineDeallocationsPerFrame = 0;
        volatile Uint64 LuaDeallocationsPerFrame = 0;

        volatile Uint64 EngineAllocations = 0;
        volatile Uint64 LuaAllocations = 0;

        volatile Uint64 EngineAllocatedBytes = 0;
        volatile Uint64 LuaAllocatedBytes = 0;

        // Nanoseconds spent on allocations/deallocations.
        volatile Uint64 EngineNSSpentOnHeapPerFrame = 0;
        volatile Uint64 LuaNSSpentOnHeapPerFrame = 0;
    }

    namespace Lua {
        volatile Uint64 CppCallsOnLuaFunctionsPerSecond = 0;
    }

    namespace Rendering {
        volatile Uint64 DrawCallsPerFrame = 0;
    }
}

void Game::Statistics::Memory::UpdateLuaStatsAllocation(Uint64 Bytes) noexcept {
    Statistics::Memory::LuaAllocatedBytes += Bytes;
    Statistics::Memory::LuaAllocationsBytesPerFrame += Bytes;
    ++Statistics::Memory::LuaAllocations;
    ++Statistics::Memory::LuaAllocationsPerFrame;
}

void Game::Statistics::Memory::UpdateLuaStatsDeallocation(
    Uint64 Bytes) noexcept {
    Statistics::Memory::LuaAllocatedBytes -= Bytes;
    --Statistics::Memory::LuaAllocations;
    ++Statistics::Memory::LuaDeallocationsPerFrame;
}

void Game::Statistics::Update() noexcept {
    using namespace Game::Statistics;

    Memory::EngineNSSpentOnHeapPerFrame = 0;
    Memory::EngineAllocationsPerFrame = 0;
    Memory::EngineAllocationBytesPerFrame = 0;
    Memory::EngineDeallocationsPerFrame = 0;

    Memory::LuaNSSpentOnHeapPerFrame = 0;
    Memory::LuaAllocationsPerFrame = 0;
    Memory::LuaAllocationsBytesPerFrame = 0;
    Memory::LuaDeallocationsPerFrame = 0;
}