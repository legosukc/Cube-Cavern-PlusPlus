#pragma once

#include <SDL3/SDL_stdinc.h>

namespace Game::Statistics {

    namespace Memory {

        extern volatile Uint64 EngineAllocationsPerFrame;
        extern volatile Uint64 LuaAllocationsPerFrame;

        extern volatile Uint64 EngineAllocationBytesPerFrame;
        extern volatile Uint64 LuaAllocationsBytesPerFrame;

        extern volatile Uint64 EngineDeallocationsPerFrame;
        extern volatile Uint64 LuaDeallocationsPerFrame;

        extern volatile Uint64 EngineAllocations;
        extern volatile Uint64 LuaAllocations;

        extern volatile Uint64 EngineAllocatedBytes;
        extern volatile Uint64 LuaAllocatedBytes;

        // Nanoseconds spent on allocations/deallocations.
        extern volatile Uint64 EngineNSSpentOnHeapPerFrame;
        extern volatile Uint64 LuaNSSpentOnHeapPerFrame;

        void UpdateLuaStatsAllocation(Uint64 Bytes) noexcept;

        void UpdateLuaStatsDeallocation(Uint64 Bytes) noexcept;
    }

    namespace Lua {
        extern volatile Uint64 CppCallsOnLuaFunctionsPerSecond;
    }

    namespace Rendering {
        extern volatile Uint64 DrawCallsPerFrame;
    }

    inline void Update() noexcept;
}