#ifndef STATISTICS_H
#define STATISTICS_H

#include <SDL3/SDL_stdinc.h>

namespace Game::Statistics {

    namespace Memory {
        inline Uint64 EngineAllocationsPerFrame = 0;
        inline Uint64 LuaAllocationsPerFrame = 0;

        inline Uint64 EngineAllocationBytesPerFrame = 0;
        inline Uint64 LuaAllocationsBytesPerFrame = 0;

        inline Uint64 EngineDeallocationsPerFrame = 0;
        inline Uint64 LuaDeallocationsPerFrame = 0;

        inline Uint64 EngineAllocations = 0;
        inline Uint64 LuaAllocations = 0;

        inline Uint64 EngineAllocatedBytes = 0;
        inline Uint64 LuaAllocatedBytes = 0;

        // Nanoseconds spent on allocations/deallocations.
        inline Uint64 EngineNSSpentOnHeapPerFrame = 0;
        inline Uint64 LuaNSSpentOnHeapPerFrame = 0;

        void UpdateLuaStatsAllocation(Uint64 Bytes) noexcept;

        void UpdateLuaStatsDeallocation(Uint64 Bytes) noexcept;
    }

    namespace Lua {
        inline Uint64 CppCallsOnLuaFunctionsPerSecond = 0;
    }

    namespace Rendering {
        inline Uint64 DrawCallsPerFrame = 0;
    }

    void Update() noexcept;
}

#endif