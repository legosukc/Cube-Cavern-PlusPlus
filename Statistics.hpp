
namespace Game::Statistics {

	namespace Memory {

		volatile Uint64 EngineAllocationsPerSecond = 0;
		volatile Uint64 LuaAllocationsPerSecond = 0;

		volatile Uint64 EngineAllocationBytesPerSecond = 0;
		volatile Uint64 LuaAllocationsBytesPerSecond = 0;

		volatile Uint64 EngineDeallocationsPerSecond = 0;
		volatile Uint64 LuaDeallocationsPerSecond = 0;

		volatile Uint64 EngineAllocations = 0;
		volatile Uint64 LuaAllocations = 0;

		volatile Uint64 EngineAllocatedBytes = 0;
		volatile Uint64 LuaAllocatedBytes = 0;

		void UpdateLuaStatsAllocation(Uint64 Bytes) {
			Statistics::Memory::LuaAllocatedBytes += Bytes;
			Statistics::Memory::LuaAllocationsBytesPerSecond += Bytes;
			++Statistics::Memory::LuaAllocations;
			++Statistics::Memory::LuaAllocationsPerSecond;
		}

		void UpdateLuaStatsDeallocation(Uint64 Bytes) {
			Statistics::Memory::LuaAllocatedBytes -= Bytes;
			--Statistics::Memory::LuaAllocations;
			++Statistics::Memory::LuaDeallocationsPerSecond;
		}
	}

	namespace Lua {
		uint64_t CppCallsOnLuaFunctionsPerSecond = 0;
	}

	namespace Rendering {
		Uint64 DrawCallsPerFrame = 0;
	}

	inline void Update();
}

namespace {
	static Uint32 _Statistics_LastTick = 0;
}
void Game::Statistics::Update() {
	using namespace Game::Statistics;

	if (const Uint32 Ticks = SDL_GetTicks(); Ticks - ::_Statistics_LastTick >= 1000) {
		::_Statistics_LastTick = Ticks;

		Memory::EngineAllocationsPerSecond = 0;
		Memory::EngineAllocationBytesPerSecond = 0;
		Memory::EngineDeallocationsPerSecond = 0;


		Memory::LuaAllocationsPerSecond = 0;
		Memory::LuaAllocationsBytesPerSecond = 0;
		Memory::LuaDeallocationsPerSecond = 0;
	}
}