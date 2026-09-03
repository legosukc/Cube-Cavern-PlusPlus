#ifndef STATISTICS_H
#define STATISTICS_H

#include <SDL3/SDL_stdinc.h>

namespace Game::Statistics {

    namespace Memory {

        extern Uint64 EngineAllocationsPerFrame;
        extern Uint64 LuaAllocationsPerFrame;

        extern Uint64 EngineAllocationBytesPerFrame;
        extern Uint64 LuaAllocationsBytesPerFrame;

        extern Uint64 EngineDeallocationsPerFrame;
        extern Uint64 LuaDeallocationsPerFrame;

        extern Uint64 EngineAllocations;
        extern Uint64 LuaAllocations;

        extern Uint64 EngineAllocatedBytes;
        extern Uint64 LuaAllocatedBytes;

        // Nanoseconds spent on allocations/deallocations.
        extern Uint64 EngineNSSpentOnHeapPerFrame;
        extern Uint64 LuaNSSpentOnHeapPerFrame;

        void UpdateLuaStatsAllocation(Uint64 Bytes);

        void UpdateLuaStatsDeallocation(Uint64 Bytes);
    }

    namespace Lua {
        extern Uint64 CppCallsOnLuaFunctionsPerSecond;
    }

    namespace Rendering {
        extern Uint64 DrawCallsPerFrame;
    }

    void Update();
}

#endif