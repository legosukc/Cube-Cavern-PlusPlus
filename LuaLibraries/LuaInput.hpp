#pragma once

#include <lua-5.5.0/lua.hpp>

#include "../FunctionHeaders/LuaHelper.hpp"

#include <iostream>


#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_scancode.h>

#include <memory>



namespace Game::Lua::CLibraries::Input {

	inline void Init(lua_State* State);

#ifndef BUILD_SERVER
	inline void Update(lua_State* State);
#endif


	static inline const char* const LibraryName = "Input";

#ifndef BUILD_SERVER
	struct Binding {
		
		static inline const char* const MetatableName = "Binding";

		const static inline size_t MaxScancodes = 24;

		Uint16 SDLScancodes[MaxScancodes];
		char BindingName[];


		static int GetBindingName(lua_State* State) {
			lua_pushstring(State, static_cast<Binding*>(LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))->BindingName);
			return 1;
		}

		static int Pressed(lua_State* State) {

			int Result = false;
			SDL_Scancode Scancode;
			Uint8 i;
			Uint16* SDLScancodes;

			SDLScancodes = static_cast<Binding*>(LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))->SDLScancodes;

			for (i = 0; i < 24; ++i) {

				Scancode = static_cast<SDL_Scancode>(SDLScancodes[i]);
				if (Scancode == SDL_SCANCODE_UNKNOWN) {
					continue;
				}

				if (Game::Window.ScancodePressed(Scancode)) {
					Result = true;
					break;
				}
			}

			lua_pushboolean(State, Result);
			return 1;
		}

		static int Held(lua_State* State) {

			int Result = false;
			SDL_Scancode Scancode;
			Uint8 i;
			Uint16* SDLScancodes;

			SDLScancodes = static_cast<Binding*>(LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))->SDLScancodes;

			for (i = 0; i < 24; ++i) {

				Scancode = static_cast<SDL_Scancode>(SDLScancodes[i]);
				if (Scancode == SDL_SCANCODE_UNKNOWN) {
					continue;
				}

				if (Game::Window.ScancodeHeld(Scancode)) {
					Result = true;
					break;
				}
			}

			lua_pushboolean(State, Result);
			return 1;
		}

		static int Released(lua_State* State) {

			int Result = false;
			SDL_Scancode Scancode;
			Uint8 i;
			Uint16* SDLScancodes;

			SDLScancodes = static_cast<Binding*>(LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))->SDLScancodes;

			for (i = 0; i < 24; ++i) {

				Scancode = static_cast<SDL_Scancode>(SDLScancodes[i]);
				if (Scancode == SDL_SCANCODE_UNKNOWN) {
					continue;
				}

				if (Game::Window.ScancodeReleased(Scancode)) {
					Result = true;
					break;
				}
			}

			lua_pushboolean(State, Result);
			return 1;
		}

		static int GetScancodes(lua_State* State) {

			int TableIndex = 0;
			Uint8 ScancodeIndex = 0;
			Uint16* SDLScancodes;

			SDLScancodes = static_cast<Binding*>(LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))->SDLScancodes;

			lua_createtable(State, 0, MaxScancodes);
			for (; ScancodeIndex < MaxScancodes; ++ScancodeIndex) {
				
				if (SDLScancodes[ScancodeIndex] != SDL_SCANCODE_UNKNOWN) {
					lua_pushinteger(State, SDLScancodes[ScancodeIndex]);
					lua_seti(State, -2, ++TableIndex);
				}
			}

			return 1;
		}

		static int AddScancode(lua_State* State) {

			Uint8 ScancodeIndex = 0;
			Uint16* SDLScancodes;
			Uint16 Scancode = luaL_checkinteger(State, 2);

			SDLScancodes = static_cast<Binding*>(LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))->SDLScancodes;

			while (true) {

				if (ScancodeIndex >= MaxScancodes) {
					luaL_error(State, "Cannot add scancode to class 'Binding'. Max scancodes reached.");
				}

				if (SDLScancodes[ScancodeIndex] == SDL_SCANCODE_UNKNOWN) {

					for (; ScancodeIndex < MaxScancodes; ++ScancodeIndex) {

						unlikely_branch
						if (SDLScancodes[ScancodeIndex] == Scancode) {
							lua_warning(State, "Attempted to add a scancode to class 'Binding' despite it already being added.", true);
							return 0;
						}
					}

					SDLScancodes[ScancodeIndex] = Scancode;
					return 0;
				}
				++ScancodeIndex;
			}
			return 0;
		}

		static int RemoveScancode(lua_State* State) {
			Uint8 ScancodeIndex;
			Uint16* SDLScancodes;
			Uint16 Scancode = luaL_checkinteger(State, 2);

			SDLScancodes = static_cast<Binding*>(LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))->SDLScancodes;

			for (ScancodeIndex = 0; ScancodeIndex < MaxScancodes; ++ScancodeIndex) {
				if (SDLScancodes[ScancodeIndex] == Scancode) {
					SDLScancodes[ScancodeIndex] = SDL_SCANCODE_UNKNOWN;
				}
			}
			return 0;
		}

		static int ClearScancodes(lua_State* State) {
			std::memset(static_cast<Binding*>(luaL_checkudata(State, 1, Binding::MetatableName))->SDLScancodes, 0, sizeof(Binding::SDLScancodes));
			return 0;
		}

		[[noreturn]]
		static int __newindex(lua_State* State) {
			luaL_error(State, "Attempted to write to a Binding object. Binding is read-only.");
		}
	};

	static int GetBinding(lua_State* State);
#endif
}


void Game::Lua::CLibraries::Input::Init(lua_State* State) {

	LuaHelper::StackTableReference EnumScancode, BindingsTable, BindingMetatable;
	//const char* KeyName;

	lua_getglobal(State, "Enums");
	lua_pushliteral(State, "Scancode");

	EnumScancode = LuaHelper::StackTableReference(State, 0, static_cast<int>(SDL_Scancode::SDL_SCANCODE_COUNT));

	// cursed fuckery to make a constant static buffer of "A\0B\0..."
	/*constexpr const static struct _AllCharactersStruct {
		constexpr _AllCharactersStruct() {

			char Letter = 'A', i = 0;

			for (; Letter <= 'Z'; ++Letter) {
				this->Characters[i] = Letter;
				this->Characters[i + 1] = '\0';
				i += 2;
			}
		}
		char Characters[2 + ('Z' - 'A') * 2]{};

	} _AllCharacters;
	
	int LetterIndex = 0;
	for (int i = 0; i < 'Z' - 'A'; ++i) {

		lua_pushlstring(State, &_AllCharacters.Characters[LetterIndex], 1);
		//lua_pushexternalstring(State, &_AllCharacters.Characters[LetterIndex], 1, NULL, NULL);
		//lua_pushlstring(State, &_AllCharacters.Characters[LetterIndex], 2);
		CLibraries::Enums::PushNewEnum(State, i + SDL_SCANCODE_A);
		lua_rawset(State, EnumScancode.GetStackIndex());
		//lua_setfield(State, EnumScancode.GetStackIndex(), &_AllCharacters.Characters[LetterIndex]);

		LetterIndex += 2;
	}*/

	//CLibraries::Enums::PushNewEnum(State, (int)SDL_SCANCODE_W);
	lua_pushinteger(State, SDL_SCANCODE_W);
	lua_setfield(State, EnumScancode.GetStackIndex(), "W");

	//CLibraries::Enums::PushNewEnum(State, SDL_SCANCODE_A);
	lua_pushinteger(State, SDL_SCANCODE_A);
	lua_setfield(State, EnumScancode.GetStackIndex(), "A");

	//CLibraries::Enums::PushNewEnum(State, SDL_SCANCODE_S);
	lua_pushinteger(State, SDL_SCANCODE_S);
	lua_setfield(State, EnumScancode.GetStackIndex(), "S");

	//CLibraries::Enums::PushNewEnum(State, SDL_SCANCODE_D);
	lua_pushinteger(State, SDL_SCANCODE_D);
	lua_setfield(State, EnumScancode.GetStackIndex(), "D");


	//CLibraries::Enums::PushNewEnum(State, SDL_SCANCODE_SPACE);
	lua_pushinteger(State, SDL_SCANCODE_SPACE);
	lua_setfield(State, EnumScancode.GetStackIndex(), "Space");

	//CLibraries::Enums::PushNewEnum(State, SDL_SCANCODE_RETURN);
	lua_pushinteger(State, SDL_SCANCODE_RETURN);
	lua_setfield(State, EnumScancode.GetStackIndex(), "Enter");

	//CLibraries::Enums::PushNewEnum(State, SDL_SCANCODE_LSHIFT);
	lua_pushinteger(State, SDL_SCANCODE_LSHIFT);
	lua_setfield(State, EnumScancode.GetStackIndex(), "LeftShift");

	//CLibraries::Enums::PushNewEnum(State, SDL_SCANCODE_RSHIFT);
	lua_pushinteger(State, SDL_SCANCODE_RSHIFT);
	lua_setfield(State, EnumScancode.GetStackIndex(), "RightShift");

	lua_rawset(State, EnumScancode.GetStackIndex() - 2);

	/*
	char Key[2] = "A";
	for (; Key[0] <= 'Z'; ++Key[0]) {
		EnumScancode.SetKey(State, CLibraries::Enums::PushNewEnum(State), Key);
	}
		
	for (Key[0] = '0'; Key[0] <= '9'; ++Key[0]) {
		EnumScancode.SetKey(State, CLibraries::Enums::PushNewEnum(State), Key);
	}
	EnumScancode.SetKey(State, CLibraries::Enums::PushNewEnum(State), "Enter");
	EnumScancode.SetKey(State, CLibraries::Enums::PushNewEnum(State), "LeftShift");
	EnumScancode.SetKey(State, CLibraries::Enums::PushNewEnum(State), "RightShift");*/
	
	/*
	for (lua_Integer Scancode = 0; Scancode < SDL_NUM_SCANCODES; ++Scancode) {

		KeyName = SDL_GetKeyName(SDL_GetKeyFromScancode(static_cast<SDL_Scancode>(Scancode)));

		if (*KeyName == '\0') {
			continue;
		}
		std::cout << "Scancode: " << Scancode << " KeyCode name: " << KeyName << '\n';
		EnumScancode.SetKey(State, Scancode, KeyName);
	}*/

#ifndef BUILD_SERVER
	BindingMetatable = LuaHelper::StackTableReference(State, CLibraries::Input::Binding::MetatableName);

	for (Uint8 i = 0; i < 7; ++i) {
		BindingMetatable.PushReference(State);
	}

	BindingMetatable.SetKeyClosure(State, Binding::GetBindingName, "GetBindingName", 1);

	BindingMetatable.SetKeyClosure(State, Binding::Pressed, "Pressed", 1);
	BindingMetatable.SetKeyClosure(State, Binding::Held, "Held", 1);
	BindingMetatable.SetKeyClosure(State, Binding::Released, "Released", 1);


	BindingMetatable.SetKeyClosure(State, Binding::GetScancodes, "GetScancodes", 1);
	BindingMetatable.SetKeyClosure(State, Binding::AddScancode, "AddScancode", 1);

	BindingMetatable.SetKeyClosure(State, Binding::RemoveScancode, "RemoveScancode", 1);
	BindingMetatable.SetKeyClosure(State, Binding::ClearScancodes, "ClearScancodes");


	BindingMetatable.SetKeyClosure(State, Binding::__newindex, "__newindex");
	//BindingMetatable.SetKey<bool>(State, false, "__metatable");


	BindingMetatable.PushReference(State);
	lua_setfield(State, BindingMetatable.GetStackIndex(), "__index");
	lua_settop(State, BindingMetatable.GetStackIndex() - 1);


	// Bindings Table
	BindingsTable = LuaHelper::StackTableReference(State, 0, 2);

	std::memset(lua_newuserdata(State, sizeof(Math::Vector2)), 0, sizeof(Math::Vector2));
	luaL_setmetatable(State, "Vector2");

	lua_setfield(State, BindingsTable.GetStackIndex(), "MouseDelta");

	std::memset(lua_newuserdata(State, sizeof(Math::Vector2)), 0, sizeof(Math::Vector2));
	luaL_setmetatable(State, "Vector2");

	lua_setfield(State, BindingsTable.GetStackIndex(), "MouseRelativePosition");

	
	lua_createtable(State, 0, 8);
	BindingsTable.SetKeyClosure(State, Input::GetBinding, "GetBinding", 1);

	LuaHelper::LockTable(State, BindingsTable.GetStackIndex());
	
	lua_setfield(State, Game::Lua::GameTable.GetStackIndex(), CLibraries::Input::LibraryName);
#endif
}


#ifndef BUILD_SERVER

void Game::Lua::CLibraries::Input::Update(lua_State* State) {

	int InputTableIndex;
	Math::Vector2* VectorUD;

	lua_getglobal(State, CLibraries::Input::LibraryName);
	InputTableIndex = lua_gettop(State);

	lua_getfield(State, InputTableIndex, "MouseDelta");
	unlikely_branch
	if (VectorUD = static_cast<Math::Vector2*>(luaL_testudata(State, -1, "Vector2")); VectorUD == NULL) {

		VectorUD = static_cast<Math::Vector2*>(lua_newuserdata(State, sizeof(Math::Vector2)));
		luaL_setmetatable(State, "Vector2");

		lua_setfield(State, InputTableIndex, "MouseDelta");
	}
	*VectorUD = Game::Window.MouseDelta;


	lua_getfield(State, InputTableIndex, "MouseRelativePosition");
	unlikely_branch
		if (VectorUD = static_cast<Math::Vector2*>(luaL_testudata(State, -1, "Vector2")); VectorUD == NULL) {

			VectorUD = static_cast<Math::Vector2*>(lua_newuserdata(State, sizeof(Math::Vector2)));
			luaL_setmetatable(State, "Vector2");

			lua_setfield(State, InputTableIndex, "MouseRelativePosition");
		}
	*VectorUD = Game::Window.MouseDelta;


	lua_getfield(State, InputTableIndex, "MouseRelativePosition");
	unlikely_branch
		if (VectorUD = static_cast<Math::Vector2*>(luaL_testudata(State, -1, "Vector2")); VectorUD == NULL) {

			VectorUD = static_cast<Math::Vector2*>(lua_newuserdata(State, sizeof(Math::Vector2)));
			luaL_setmetatable(State, "Vector2");

			lua_setfield(State, InputTableIndex, "MouseRelativePosition");
		}
	*VectorUD = Game::Window.MouseDelta;

	lua_settop(State, InputTableIndex - 1);
}




int Game::Lua::CLibraries::Input::GetBinding(lua_State* State) {

	Input::Binding* BindingUD;
	size_t BindingNameLen;
	const char* BindingName;

	BindingName = luaL_checklstring(State, 1, &BindingNameLen);
	if (lua_getfield(State, lua_upvalueindex(1), BindingName) != LUA_TUSERDATA) {
		
		BindingUD = static_cast<Input::Binding*>(lua_newuserdata(State, sizeof(Input::Binding) + BindingNameLen));
		std::memset(&BindingUD->SDLScancodes, 0, sizeof(Input::Binding::SDLScancodes));
		std::memcpy(&BindingUD->BindingName, BindingName, BindingNameLen);

		luaL_setmetatable(State, CLibraries::Input::Binding::MetatableName);

		lua_pushvalue(State, -1);
		lua_setfield(State, lua_upvalueindex(1), BindingName);
	}
	/*
	std::cout << "stack top: " << luaL_typename(State, -1) << std::endl;
	lua_rotate(State, 1, 1);

	std::cout << "returning: " << luaL_typename(State, 1) << std::endl;*/
	return 1;
}
#endif