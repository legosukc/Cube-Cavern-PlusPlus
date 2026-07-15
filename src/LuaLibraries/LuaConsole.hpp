#pragma once

#include <iostream>
#include <string>


#include <lua.hpp>
#include "../FunctionHeaders/LuaHelper.hpp"

#include <SDL3/SDL_thread.h>
#include <SDL3/SDL_stdinc.h>


namespace Game::Lua::CLibraries::Console {

	inline void Init(lua_State* State);
	inline void PostMiscClassInit(lua_State* State);

	inline void DestroyLuaLibrary();
	inline void DestroyLibrary();

	inline void Update(lua_State* State);

	static inline const char* const LibraryName = "Console";
}


void Game::Lua::CLibraries::Console::Init(lua_State* State) {

	lua_createtable(State, 0, 8);
	lua_setfield(State, -2, "");
}


namespace {

	namespace _LuaConsole {

		static SDL_Thread* _OnPromptThread;

		static bool _RunThread = true;
		static volatile bool _NewPrompt = false;
		static std::string _Prompt;

		static int _OnPromptThreadFunc(void*) {

			if (!SDL_SetCurrentThreadPriority(SDL_ThreadPriority::SDL_THREAD_PRIORITY_LOW)) {
				std::clog << "Failed to set thread priority for _OnPromptThreadFunc." << std::endl;
			}

			while (::_LuaConsole::_RunThread) {
				std::getline(std::cin, ::_LuaConsole::_Prompt);
				::_LuaConsole::_NewPrompt = true;
			}

			return -1;
		}
	}
}

void Game::Lua::CLibraries::Console::PostMiscClassInit(lua_State* State) {

	LuaHelper::StackTableReference ConsoleTable(State, 0, 2);

	lua_getglobal(State, "Assets");
	lua_getfield(State, -1, "MiscClassesGlobal");
	lua_getfield(State, -1, "BindableEvent");
	lua_getfield(State, -1, "new");
	lua_call(State, 0, 1);

	lua_setfield(State, ConsoleTable.GetStackIndex(), "OnPrompt");
	lua_settop(State, ConsoleTable.GetStackIndex());
	lua_setglobal(State, CLibraries::Console::LibraryName);

	::_LuaConsole::_OnPromptThread = SDL_CreateThread(::_LuaConsole::_OnPromptThreadFunc, "_OnPromptThread", NULL);
}

void Game::Lua::CLibraries::Console::Update(lua_State* State) {
	
	if (::_LuaConsole::_NewPrompt) {
		::_LuaConsole::_NewPrompt = false;
		
		lua_getglobal(State, CLibraries::Console::LibraryName);
		lua_getfield(State, -1, "OnPrompt");
		lua_pushstring(State, ::_LuaConsole::_Prompt.c_str());
		lua_getfield(State, -1, "Fire");
		lua_call(State, 2, 0);

		lua_pop(State, 1);
	}
}

void Game::Lua::CLibraries::Console::DestroyLibrary() {

	::_LuaConsole::_RunThread = false;
	SDL_WaitThread(::_LuaConsole::_OnPromptThread, NULL);
}