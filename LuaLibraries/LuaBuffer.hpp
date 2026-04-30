#pragma once

#include <lua-5.4.2/lua.hpp>

#include <SDL2/SDL_stdinc.h>



namespace Game::Lua::CLibraries::Buffer {

	inline void Init(lua_State* State);

	static int Create(lua_State* State);
	static int FromString(lua_State* State);

	struct Buffer;
}

struct Game::Lua::CLibraries::Buffer::Buffer {
	static inline void InitMetatable(lua_State* State);

	Uint32 Size;
	void* Data;

	static int GetSize(lua_State* State) {
		lua_pushinteger(State, static_cast<lua_Integer>(static_cast<Buffer*>(lua_touserdata(State, 1))->Size));
		lua_rotate(State, 0, 1);
		return 1;
	}

	static int Reserve(lua_State* State) {
		void* OldData;
		Uint32 OldSize;
		Buffer* BufferUD;
		lua_Integer ReserveCount;

		BufferUD = static_cast<Buffer*>(lua_touserdata(State, 1));
		ReserveCount = luaL_checkinteger(State, 2);

		unlikely_branch
			if (ReserveCount < 0) {
				luaL_error(State, "Attempted to reserve by a negative amount of bytes on a 'buffer' class.");
			}

		OldSize = BufferUD->Size;
		if (static_cast<Uint64>(BufferUD->Size) + static_cast<Uint64>(ReserveCount) > UINT32_MAX) {
			luaL_error(State, "Cannot reserve buffer, size would go past the unsigned 32 bit integer limit.");
		}
		BufferUD->Size += static_cast<Uint32>(ReserveCount);

		OldData = BufferUD->Data;
		BufferUD->Data = std::malloc(BufferUD->Size);

		std::memcpy(BufferUD->Data, OldData, OldSize);
		std::free(OldData);

		return 0;
	}
	static int Shrink(lua_State* State) {
		void* OldData;
		Buffer* BufferUD;
		lua_Integer ShrinkAmount;

		BufferUD = static_cast<Buffer*>(lua_touserdata(State, 1));
		ShrinkAmount = luaL_checkinteger(State, 2);

		unlikely_branch
			if (ShrinkAmount < 0) {
				luaL_error(State, "Attempted to shrink by a negative amount of bytes on a 'buffer' class.");
			}

		unlikely_branch
			if (static_cast<Uint64>(ShrinkAmount) >= BufferUD->Size) {
				luaL_error(State, "Attempted to shrink a 'buffer' class into the negatives.");
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

		Buffer* BufferUD;
		lua_Integer Index;

		Index = luaL_checkinteger(State, 2);
		unlikely_branch
			if (Index < 0) {
				luaL_error(State, "Invalid buffer index: Index is less than 0.");
			}

		BufferUD = static_cast<Buffer*>(lua_touserdata(State, 1));

		unlikely_branch
			if (Index + sizeof(T) > static_cast<lua_Integer>(BufferUD->Size)) {
				luaL_error(State, "Invalid buffer index: Index exceeds buffer size, which is ", BufferUD->Size);
			}

		if CONSTEXPR_IF(std::is_integral_v<T>) {
			*(static_cast<T*>(BufferUD->Data) + Index) = static_cast<T>(luaL_checkinteger(State, 3));
		} else if CONSTEXPR_IF(std::is_floating_point_v<T>) {
			*(static_cast<T*>(BufferUD->Data) + Index) = static_cast<T>(luaL_checknumber(State, 3));
		}
		return 0;
	}

	template<typename T>
	static int Read(lua_State* State) {
		static_assert(
			std::is_integral_v<T> || std::is_floating_point_v<T>,
			"Invalid template type parameter, expected integral or floating point."
			);

		Buffer* BufferUD;
		lua_Integer Index;

		Index = luaL_checkinteger(State, 2);
		unlikely_branch
			if (Index < 0) {
				luaL_error(State, "Invalid buffer index: Index is less than 0.");
			}

		BufferUD = static_cast<Buffer*>(lua_touserdata(State, 1));

		unlikely_branch
			if (Index + sizeof(T) > static_cast<lua_Integer>(BufferUD->Size)) {
				luaL_error(State, "Invalid buffer index: Index exceeds buffer size, which is ", BufferUD->Size);
			}

		if CONSTEXPR_IF(std::is_integral_v<T>) {
			lua_pushinteger(State, static_cast<lua_Integer>(*(static_cast<T*>(BufferUD->Data) + Index)));
		} else if CONSTEXPR_IF(std::is_floating_point_v<T>) {
			lua_pushnumber(State, static_cast<lua_Number>(*(static_cast<T*>(BufferUD->Data) + Index)));
		}
		lua_rotate(State, 0, 1);
		return 1;
	}

	static int ToString(lua_State* State) {

		Buffer* BufferUD = static_cast<Buffer*>(lua_touserdata(State, 1));
		lua_pushlstring(State, static_cast<const char*>(BufferUD->Data), static_cast<size_t>(BufferUD->Size));

		lua_rotate(State, 0, 1);
		return 1;
	}

	static int __gc(lua_State* State) {

		Buffer* const BufferUD = static_cast<Buffer*>(lua_touserdata(State, 1));
		std::free(BufferUD->Data);
		Statistics::Memory::UpdateLuaStatsDeallocation(BufferUD->Size);
		return 0;
	}
};


void Game::Lua::CLibraries::Buffer::Init(lua_State* State) {

	Buffer::InitMetatable(State);

	LuaHelper::StackTableReference BufferTable(State, 0, 2);

	BufferTable.SetKey(State, Create, "Create");
	BufferTable.SetKey(State, FromString, "FromString");

	lua_setglobal(State, "buffer");
}




void Game::Lua::CLibraries::Buffer::Buffer::InitMetatable(lua_State* State) {

	LuaHelper::StackTableReference BufferMetatable(State, "Buffer");

	BufferMetatable.SetKey(State, Buffer::GetSize, "Size");

	BufferMetatable.SetKey(State, Buffer::ToString, "ToString");

	BufferMetatable.SetKey(State, Buffer::Reserve, "Reserve");
	BufferMetatable.SetKey(State, Buffer::Shrink, "Shrink");


	BufferMetatable.SetKey(State, Buffer::Write<Sint8>, "Writei8");
	BufferMetatable.SetKey(State, Buffer::Write<Sint16>, "Writei16");
	BufferMetatable.SetKey(State, Buffer::Write<Sint32>, "Writei32");
	BufferMetatable.SetKey(State, Buffer::Write<Sint64>, "Writei64");

	BufferMetatable.SetKey(State, Buffer::Write<Uint8>, "Writeu8");
	BufferMetatable.SetKey(State, Buffer::Write<Uint16>, "Writeu16");
	BufferMetatable.SetKey(State, Buffer::Write<Uint32>, "Writeu32");
	BufferMetatable.SetKey(State, Buffer::Write<Uint64>, "Writeu64");

	BufferMetatable.SetKey(State, Buffer::Write<float>, "Writef32");
	BufferMetatable.SetKey(State, Buffer::Write<double>, "Writef64");


	BufferMetatable.SetKey(State, Buffer::Read<Sint8>, "Readi8");
	BufferMetatable.SetKey(State, Buffer::Read<Sint16>, "Readi16");
	BufferMetatable.SetKey(State, Buffer::Read<Sint32>, "Readi32");
	BufferMetatable.SetKey(State, Buffer::Read<Sint64>, "Readi64");

	BufferMetatable.SetKey(State, Buffer::Read<Uint8>, "Readu8");
	BufferMetatable.SetKey(State, Buffer::Read<Uint16>, "Readu16");
	BufferMetatable.SetKey(State, Buffer::Read<Uint32>, "Readu32");
	BufferMetatable.SetKey(State, Buffer::Read<Uint64>, "Readu64");

	BufferMetatable.SetKey(State, Buffer::Read<float>, "Readf32");
	BufferMetatable.SetKey(State, Buffer::Read<double>, "Readf64");

	BufferMetatable.SetKey(State, Buffer::__gc, "__gc");

	lua_pushvalue(State, BufferMetatable.GetStackIndex());
	lua_setfield(State, BufferMetatable.GetStackIndex(), "__index");

	lua_settop(State, BufferMetatable.GetStackIndex() - 1);
}


namespace {
	static Game::Lua::CLibraries::Buffer::Buffer* allocateBuffer(lua_State* State, lua_Integer Size) {
		Game::Lua::CLibraries::Buffer::Buffer* BufferUD;

		unlikely_branch
		if (Size < 0) {
			luaL_error(State, "Invalid buffer size: Size is less than 0.");
		}

		unlikely_branch
		if (Size > UINT32_MAX) {
			luaL_error(State, "Attempted to allocate a buffer with a size past the unsigned 32 bit integer limit.");
		}

		BufferUD = static_cast<Game::Lua::CLibraries::Buffer::Buffer*>(lua_newuserdata(State, sizeof(Game::Lua::CLibraries::Buffer::Buffer)));
		BufferUD->Size = static_cast<Uint32>(Size);
		BufferUD->Data = std::malloc(BufferUD->Size);

		//Game::Statistics::Memory::UpdateLuaStatsAllocation(BufferUD->Size);

		lua_rotate(State, 0, 1);
		return BufferUD;
	}
}


static int Game::Lua::CLibraries::Buffer::Create(lua_State* State) {
	lua_Integer Size;

	Size = luaL_checkinteger(State, 1);
	unlikely_branch
	if (Size < 0) {
		luaL_error(State, "Invalid buffer size: Size is less than 0.");
	}

	unlikely_branch
	if (Size > UINT32_MAX) {
		luaL_error(State, "Attempted to allocate a buffer with a size past the unsigned 32 bit integer limit.");
	}

	std::memset(::allocateBuffer(State, static_cast<size_t>(Size))->Data, 0, static_cast<size_t>(Size));

	return 1;
}

static int Game::Lua::CLibraries::Buffer::FromString(lua_State* State) {
	const char* String;
	size_t Size;

	String = luaL_checklstring(State, 1, &Size);

	std::memcpy(::allocateBuffer(State, Size)->Data, String, Size);

	return 1;
}