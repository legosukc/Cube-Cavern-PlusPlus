#pragma once

#include "../define.h"

#include <memory>

#include <lua.hpp>
#include "../FunctionHeaders/LuaHelper.hpp"

#include <SDL3/SDL_stdinc.h>

#include "../Statistics.hpp"


namespace Game::Lua::CLibraries::Buffer {

	inline void Init(lua_State* State);

	static int __new(lua_State* State);
	static int FromString(lua_State* State);

	inline const char* const LibraryName = "buffer";

	namespace Classes {
		struct Buffer;
	}
}

struct Game::Lua::CLibraries::Buffer::Classes::Buffer {

	Uint32 Size;
	void* Data;

	static inline const char* const MetatableName = "buffer";

	static int GetSize(lua_State* State) {
		lua_pushinteger(State, 							static_cast<lua_Integer>(static_cast<Classes::Buffer*>(luaL_checkudata(State, 1, Buffer::MetatableName))->Size));
		return 1;
	}

	static int Reserve(lua_State* State) {
		void* OldData;
		Uint32 OldSize;
		Classes::Buffer* BufferUD;
		lua_Integer ReserveCount;

		BufferUD = static_cast<Classes::Buffer*>(lua_touserdata(State, 1));
		ReserveCount = luaL_checkinteger(State, 2);

		unlikely_branch
		if (ReserveCount < 0) {
			luaL_error(State, "Attempted to reserve a negative amount of bytes on a 'buffer' class.");
		}

		OldSize = BufferUD->Size;
		if (static_cast<Uint64>(BufferUD->Size) + static_cast<Uint64>(ReserveCount) > UINT32_MAX) {
			luaL_error(State, "Attempted to reserve a 'buffer' class with a byte size past the unsigned 32 bit integer limit.");
		}
		BufferUD->Size += static_cast<Uint32>(ReserveCount);

		OldData = BufferUD->Data;
		BufferUD->Data = std::malloc(BufferUD->Size);

		++Game::Statistics::Memory::LuaAllocationsPerFrame;
		Game::Statistics::Memory::LuaAllocationsBytesPerFrame += static_cast<Uint64>(ReserveCount);
		Game::Statistics::Memory::LuaAllocatedBytes += static_cast<Uint64>(ReserveCount);

		std::memcpy(BufferUD->Data, OldData, OldSize);
		std::free(OldData);

		++Game::Statistics::Memory::LuaDeallocationsPerFrame;

		return 0;
	}
	static int Shrink(lua_State* State) {
		void* OldData;
		Buffer* BufferUD;
		lua_Integer ShrinkAmount;

		BufferUD = static_cast<Classes::Buffer*>(luaL_checkudata(State, 1, Buffer::MetatableName));
		ShrinkAmount = luaL_checkinteger(State, 2);

		unlikely_branch
		if (ShrinkAmount < 0) {
			luaL_error(State, "Attempted to shrink a negative amount of bytes on a 'buffer' class.");
		}

		unlikely_branch
		if (static_cast<Uint64>(ShrinkAmount) >= BufferUD->Size) {
			luaL_error(State, "Attempted to shrink a 'buffer' class more than its allocated size.");
		}

		BufferUD->Size -= static_cast<Uint64>(ShrinkAmount);

		OldData = BufferUD->Data;
		BufferUD->Data = std::malloc(BufferUD->Size);

		std::memcpy(BufferUD->Data, OldData, BufferUD->Size);
		std::free(OldData);

		return 0;
	}

	template<typename T>
	static int Write(lua_State* State) {
		static_assert(
			std::is_integral_v<T> || std::is_floating_point_v<T>,
			"Invalid template type parameter, expected integral or floating point."
			);

		Classes::Buffer* BufferUD;
		lua_Integer Index;

		Index = luaL_checkinteger(State, 2);
		unlikely_branch
			if (Index < 0) {
				luaL_error(State, "Invalid buffer write index: Index is less than 0.");
			}

		BufferUD = static_cast<Classes::Buffer*>(luaL_checkudata(State, 1, Buffer::MetatableName));

		unlikely_branch
		if (Index + sizeof(T) > static_cast<lua_Integer>(BufferUD->Size)) {
			luaL_error(State, "Invalid buffer write index: Index exceeds buffer size, which is ", BufferUD->Size);
		}

		*(static_cast<T*>(BufferUD->Data) + Index) = LuaHelper::ToTypename<T>(State, 3);
		return 0;
	}

	template<typename T>
	static int Read(lua_State* State) {
		static_assert(
			std::is_integral_v<T> || std::is_floating_point_v<T>,
			"Invalid template type parameter, expected integral or floating point."
			);

		Classes::Buffer* BufferUD;
		lua_Integer Index;

		Index = luaL_checkinteger(State, 2);
		unlikely_branch
		if (Index < 0) {
			luaL_error(State, "Invalid buffer read index: Index is less than 0.");
		}

		BufferUD = static_cast<Classes::Buffer*>(luaL_checkudata(State, 1, Buffer::MetatableName));

		unlikely_branch
		if (Index + sizeof(T) > static_cast<lua_Integer>(BufferUD->Size)) {
			luaL_error(State, "Invalid buffer read index: Index reads past buffer size, which is ", BufferUD->Size);
		}

		LuaHelper::Push<T>(State, *(static_cast<T*>(BufferUD->Data) + Index));
		return 1;
	}

	static int ToString(lua_State* State) {

		Classes::Buffer* BufferUD = static_cast<Classes::Buffer*>(luaL_checkudata(State, 1, Buffer::MetatableName));
		lua_pushlstring(State, static_cast<const char*>(BufferUD->Data), static_cast<size_t>(BufferUD->Size));
		return 1;
	}

	static int __gc(lua_State* State) {

		Classes::Buffer* const BufferUD = static_cast<Classes::Buffer*>(lua_touserdata(State, 1));
		std::free(BufferUD->Data);
		Statistics::Memory::UpdateLuaStatsDeallocation(BufferUD->Size);
		return 0;
	}
};


void Game::Lua::CLibraries::Buffer::Init(lua_State* State) {

	LuaHelper::StackTableReference BufferTable, BufferMetatable;

	BufferMetatable = LuaHelper::StackTableReference(State, Classes::Buffer::MetatableName);

	BufferMetatable.SetKeyClosure(State, Classes::Buffer::GetSize, "Size");

	BufferMetatable.SetKeyClosure(State, Classes::Buffer::ToString, "ToString");

	BufferMetatable.SetKeyClosure(State, Classes::Buffer::Reserve, "Reserve");
	BufferMetatable.SetKeyClosure(State, Classes::Buffer::Shrink, "Shrink");


	BufferMetatable.SetKeyClosure(State, Classes::Buffer::Write<Sint8>, "Writei8");
	BufferMetatable.SetKeyClosure(State, Classes::Buffer::Write<Sint16>, "Writei16");
	BufferMetatable.SetKeyClosure(State, Classes::Buffer::Write<Sint32>, "Writei32");
	BufferMetatable.SetKeyClosure(State, Classes::Buffer::Write<Sint64>, "Writei64");

	BufferMetatable.SetKeyClosure(State, Classes::Buffer::Write<Uint8>, "Writeu8");
	BufferMetatable.SetKeyClosure(State, Classes::Buffer::Write<Uint16>, "Writeu16");
	BufferMetatable.SetKeyClosure(State, Classes::Buffer::Write<Uint32>, "Writeu32");
	BufferMetatable.SetKeyClosure(State, Classes::Buffer::Write<Uint64>, "Writeu64");

	BufferMetatable.SetKeyClosure(State, Classes::Buffer::Write<float>, "Writef32");
	BufferMetatable.SetKeyClosure(State, Classes::Buffer::Write<double>, "Writef64");


	BufferMetatable.SetKeyClosure(State, Classes::Buffer::Read<Sint8>, "Readi8");
	BufferMetatable.SetKeyClosure(State, Classes::Buffer::Read<Sint16>, "Readi16");
	BufferMetatable.SetKeyClosure(State, Classes::Buffer::Read<Sint32>, "Readi32");
	BufferMetatable.SetKeyClosure(State, Classes::Buffer::Read<Sint64>, "Readi64");

	BufferMetatable.SetKeyClosure(State, Classes::Buffer::Read<Uint8>, "Readu8");
	BufferMetatable.SetKeyClosure(State, Classes::Buffer::Read<Uint16>, "Readu16");
	BufferMetatable.SetKeyClosure(State, Classes::Buffer::Read<Uint32>, "Readu32");
	BufferMetatable.SetKeyClosure(State, Classes::Buffer::Read<Uint64>, "Readu64");

	BufferMetatable.SetKeyClosure(State, Classes::Buffer::Read<float>, "Readf32");
	BufferMetatable.SetKeyClosure(State, Classes::Buffer::Read<double>, "Readf64");

	BufferMetatable.SetKeyClosure(State, Classes::Buffer::__gc, "__gc");

	BufferMetatable.PushReference(State);
	lua_setfield(State, BufferMetatable.GetStackIndex(), "__index");

	lua_settop(State, BufferMetatable.GetStackIndex() - 1);

	BufferTable = LuaHelper::StackTableReference(State, 0, 2);

	BufferTable.SetKeyClosure(State, Buffer::__new, "new");
	BufferTable.SetKeyClosure(State, Buffer::FromString, "FromString");

	lua_setglobal(State, CLibraries::Buffer::LibraryName);
}






namespace {
	static Game::Lua::CLibraries::Buffer::Classes::Buffer* allocateBuffer(lua_State* State, lua_Integer Size) {
		using BufferUDClass = Game::Lua::CLibraries::Buffer::Classes::Buffer;

		BufferUDClass* BufferUD;

		unlikely_branch
		if (Size < 0) {
			luaL_error(State, "Invalid buffer size: Size is less than 0.");
		}

		unlikely_branch
		if (Size > UINT32_MAX) {
			luaL_error(State, "Attempted to allocate a buffer with a size past the unsigned 32 bit integer limit.");
		}

		BufferUD = static_cast<BufferUDClass*>(lua_newuserdata(State, sizeof(BufferUDClass)));
		BufferUD->Size = static_cast<Uint32>(Size);
		BufferUD->Data = std::malloc(BufferUD->Size);

		luaL_setmetatable(State, BufferUDClass::MetatableName);


		//Game::Statistics::Memory::UpdateLuaStatsAllocation(BufferUD->Size);

		return BufferUD;
	}
}


static int Game::Lua::CLibraries::Buffer::__new(lua_State* State) {

	const lua_Integer Size = luaL_checkinteger(State, 1);
	std::memset(::allocateBuffer(State, Size)->Data, 0, Size);

	return 1;
}

static int Game::Lua::CLibraries::Buffer::FromString(lua_State* State) {
	const char* String;
	size_t Size;

	String = luaL_checklstring(State, 1, &Size);

	std::memcpy(::allocateBuffer(State, Size)->Data, String, Size);

	return 1;
}
