#include "Statistics.hpp"

#include <SDL3/SDL_stdinc.h>

void Game::Statistics::Memory::UpdateLuaStatsAllocation(Uint64 Bytes) noexcept {
    Statistics::Memory::LuaAllocatedBytes += Bytes;
    Statistics::Memory::LuaAllocationsBytesPerFrame += Bytes;
    ++Statistics::Memory::LuaAllocations;
    ++Statistics::Memory::LuaAllocationsPerFrame;
}

void Game::Statistics::Memory::UpdateLuaStatsDeallocation(Uint64 Bytes) noexcept {
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