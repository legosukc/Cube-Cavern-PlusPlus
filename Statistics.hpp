#pragma once

#include <SDL2/SDL_stdinc.h>


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


		// Microseconds spent on allocations/deallocations.
		volatile Uint64 EngineMicrosecondsSpentOnHeapPerFrame = 0;
		volatile Uint64 LuaMicrosecondsSpentOnHeapPerFrame = 0;

		void UpdateLuaStatsAllocation(Uint64 Bytes) {
			Statistics::Memory::LuaAllocatedBytes += Bytes;
			Statistics::Memory::LuaAllocationsBytesPerFrame += Bytes;
			++Statistics::Memory::LuaAllocations;
			++Statistics::Memory::LuaAllocationsPerFrame;
		}

		void UpdateLuaStatsDeallocation(Uint64 Bytes) {
			Statistics::Memory::LuaAllocatedBytes -= Bytes;
			--Statistics::Memory::LuaAllocations;
			++Statistics::Memory::LuaDeallocationsPerFrame;
		}
	}

	namespace Lua {
		Uint64 CppCallsOnLuaFunctionsPerSecond = 0;
	}

	namespace Rendering {
		Uint64 DrawCallsPerFrame = 0;
	}

	inline void Update();
}


void Game::Statistics::Update() {
	using namespace Game::Statistics;

	Memory::EngineMicrosecondsSpentOnHeapPerFrame = 0;
	Memory::EngineAllocationsPerFrame = 0;
	Memory::EngineAllocationBytesPerFrame = 0;
	Memory::EngineDeallocationsPerFrame = 0;


	Memory::LuaMicrosecondsSpentOnHeapPerFrame = 0;
	Memory::LuaAllocationsPerFrame = 0;
	Memory::LuaAllocationsBytesPerFrame = 0;
	Memory::LuaDeallocationsPerFrame = 0;
}