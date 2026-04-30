#pragma once

#include <lua-5.4.2/lua.hpp>

#include "../../FunctionHeaders/LuaHelper.hpp"

#include <iostream>


#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_scancode.h>

#include <memory>



namespace Game::Lua::CLibraries::Input {

	inline void Init(lua_State* State);

	static int Update(lua_State* State);


	struct Binding {
		static inline void InitMetatable(lua_State* State);

		Uint16 SDLScancodes[16];
		char BindingName[];


		static int GetBindingName(lua_State* State);

		static int GetKeycodes(lua_State* State);
		static int AddKeycode(lua_State* State);
		static int RemoveKeycode(lua_State* State);
		static int ClearKeycodes(lua_State* State);

		static int __eq(lua_State* State) {
			return 1;
		}

		[[noreturn]]
		static int __newindex(lua_State* State) {
			luaL_error(State, "Attempted to write to a Binding object. Binding is read-only.");
		}
	};

	static int GetBinding(lua_State* State);
}


void Game::Lua::CLibraries::Input::Init(lua_State* State) {

	LuaHelper::StackTableReference EnumScancode(State, 0, SDL_NUM_SCANCODES);

	const char* KeyName;
	
	for (lua_Integer Scancode = 0; Scancode < SDL_NUM_SCANCODES; ++Scancode) {

		KeyName = SDL_GetKeyName(SDL_GetKeyFromScancode(static_cast<SDL_Scancode>(Scancode)));
		std::cout << "Scancode: " << Scancode << " KeyCode name: " << KeyName << '\n';

		if (*KeyName == '\0') {
			continue;
		}
		EnumScancode.SetKey(State, Scancode, KeyName);
	}

	lua_setglobal(State, "Enum.Scancode");


	Binding::InitMetatable(State);


	// Bindings Table
	lua_createtable(State, 0, 16);

	lua_pushcclosure(State, Input::GetBinding, 1);
	lua_setfield(State, -2, "GetBinding");

	lua_setglobal(State, "Input");
}


void Game::Lua::CLibraries::Input::Binding::InitMetatable(lua_State* State) {

	LuaHelper::StackTableReference BindingMetatable(State, "Binding");

	BindingMetatable.SetKey<bool>(State, false, "Pressed");
	BindingMetatable.SetKey<bool>(State, false, "Held");

	BindingMetatable.SetKeyClosure(State, Binding::__newindex, "__newindex");
	BindingMetatable.SetKeyClosure(State, Binding::__eq, "__eq");
	BindingMetatable.SetKey<bool>(State, false, "__metatable");

	lua_settop(State, BindingMetatable.GetStackIndex() - 1);
}


int Game::Lua::CLibraries::Input::GetBinding(lua_State* State) {

	lua_settop(State, 1);

	size_t BindingNameLen;
	const char* BindingName = luaL_checklstring(State, 1, &BindingNameLen);
	if (lua_gettable(State, lua_upvalueindex(1)) == LUA_TNIL) {

		Input::Binding* const BindingUD = static_cast<Input::Binding*>(lua_newuserdata(State, sizeof(Input::Binding) + BindingNameLen));
		std::memset(&BindingUD->SDLScancodes, 0, sizeof(Input::Binding::SDLScancodes));
		std::memcpy(&BindingUD->BindingName, BindingName, BindingNameLen);

		luaL_setmetatable(State, "Binding");

		lua_pushvalue(State, 1);
		lua_pushvalue(State, -1);

		lua_settable(State, lua_upvalueindex(1));
	}

	lua_rotate(State, 0, 1);
	return 1;
}