#pragma once

#include <lua-5.5.0/lua.hpp>
#include "../FunctionHeaders/LuaHelper.hpp"

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>

#include "LuaBuffer.hpp"


namespace Game::Lua::CLibraries::Surface {

	inline void Init(lua_State* State);

	namespace {
		template<typename ColorType, Uint8 RBits = 0, Uint8 GBits = 0, Uint8 BBits = 0, Uint8 ABits = 0>
		struct _FormatBase {
			constexpr static inline const Uint8 RedBits = RBits;
			constexpr static inline const Uint8 GreenBits = GBits;
			constexpr static inline const Uint8 BlueBits = BBits;
			constexpr static inline const Uint8 AlphaBits = ABits;

			using Type = ColorType;
		};
	}


	using R8I  = _FormatBase<Sint8, 8>;
	using R16I = _FormatBase<Sint16, 16>;
	using R32I = _FormatBase<Sint32, 32>;

	using R8UI  = _FormatBase<Uint8, 8>;
	using R16UI = _FormatBase<Uint16, 16>;
	using R32UI = _FormatBase<Uint32, 32>;

	using R32F = _FormatBase<float, 32>;


	using RG8I  = _FormatBase<Sint8, 8, 8>;
	using RG16I = _FormatBase<Sint16, 16, 16>;
	using RG32I = _FormatBase<Sint32, 32, 32>;

	using RG8UI  = _FormatBase<Uint8, 8, 8>;
	using RG16UI = _FormatBase<Uint16, 16, 16>;
	using RG32UI = _FormatBase<Uint32, 32, 32>;

	using RG32F = _FormatBase<float, 32, 32>;


	using RGB8I  = _FormatBase<Sint8, 8, 8, 8>;
	using RGB16I = _FormatBase<Sint16, 16, 16, 16>;
	using RGB32I = _FormatBase<Sint32, 32, 32, 32>;

	using RGB8UI  = _FormatBase<Uint8, 8, 8, 8>;
	using RGB16UI = _FormatBase<Uint16, 16, 16, 16>;
	using RGB32UI = _FormatBase<Uint32, 32, 32, 32>;

	using RGB32F = _FormatBase<float, 32, 32, 32>;


	using RGBA8I  = _FormatBase<Sint8, 8, 8, 8, 8>;
	using RGBA16I = _FormatBase<Sint16, 16, 16, 16, 16>;
	using RGBA32I = _FormatBase<Sint32, 32, 32, 32, 32>;

	using RGBA8UI  = _FormatBase<Uint8, 8, 8, 8, 8>;
	using RGBA16UI = _FormatBase<Uint16, 16, 16, 16, 16>;
	using RGBA32UI = _FormatBase<Uint32, 32, 32, 32, 32>;

	using RGBA32F = _FormatBase<float, 32, 32, 32, 32>;


	static int BufferLoadImage(lua_State* State);
	static int ConvertBufferTo(lua_State* State);

	static inline const char* const LibraryName = "Surface";
}


void Game::Lua::CLibraries::Surface::Init(lua_State* State) {

	LuaHelper::StackTableReference BufferTable = LuaHelper::StackTableReference(State, 0, 2);

	BufferTable.SetKeyClosure(State, Surface::BufferLoadImage, "BufferLoadImage");
	BufferTable.SetKeyClosure(State, Surface::ConvertBufferTo, "ConvertBufferTo");

	lua_setglobal(State, CLibraries::Surface::LibraryName);
}



int Game::Lua::CLibraries::Surface::BufferLoadImage(lua_State* State) {

	SDL_Surface* Surface = IMG_Load(luaL_checkstring(State, 1));

	using BufferUDClass = Game::Lua::CLibraries::Buffer::Classes::Buffer;
	
	lua_createtable(State, 0, 2);

	BufferUDClass* BufferUD = static_cast<BufferUDClass*>(lua_newuserdata(State, sizeof(BufferUDClass)));
	BufferUD->Size = static_cast<Uint32>(Surface->h * Surface->pitch);
	BufferUD->Data = std::malloc(BufferUD->Size);
	std::memcpy(BufferUD->Data, Surface->pixels, BufferUD->Size);

	luaL_setmetatable(State, BufferUDClass::MetatableName);

	lua_setfield(State, -2, "Buffer");

	lua_pushinteger(State, Surface->format);
	lua_setfield(State, -2, "Format");

	SDL_DestroySurface(Surface);

	return 1;
}

int Game::Lua::CLibraries::Surface::ConvertBufferTo(lua_State* State) {

	SDL_Surface* Surface = IMG_Load(luaL_checkstring(State, 1));

	using BufferUDClass = Game::Lua::CLibraries::Buffer::Classes::Buffer;

	BufferUDClass* BufferUD = static_cast<BufferUDClass*>(lua_newuserdata(State, sizeof(BufferUDClass)));
	BufferUD->Size = static_cast<Uint32>(Surface->h * Surface->pitch);
	BufferUD->Data = std::malloc(BufferUD->Size);
	std::memcpy(BufferUD->Data, Surface->pixels, BufferUD->Size);

	SDL_DestroySurface(Surface);

	luaL_setmetatable(State, BufferUDClass::MetatableName);

	return 1;
}