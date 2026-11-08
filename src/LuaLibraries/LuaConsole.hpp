#pragma once

#include "../../include/VM/lua.h"

#include <string>

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_thread.h>

namespace Game::Lua::CLibraries::Console {

    namespace etc {
        extern SDL_Thread* _OnPromptThread;

        extern volatile bool _RunThread;
        extern volatile bool _NewPrompt;
        extern std::string _Prompt;

        int _OnPromptThreadFunc(void*);
    }

    inline void Init(lua_State* State);
    inline void PostMiscClassInit(lua_State* State);

    inline void DestroyLuaLibrary();
    inline void DestroyLibrary();

    inline void Update(lua_State* State);

    static constexpr const char* LibraryName = "Console";
}

void Game::Lua::CLibraries::Console::Init(lua_State* State) {
    lua_createtable(State, 0, 8);
    lua_setfield(State, -2, "");
}

void Game::Lua::CLibraries::Console::PostMiscClassInit(lua_State* State) {
    using namespace Game::Lua::CLibraries::Console::etc;
    lua_createtable(State, 0, 2);
    const int TableIndex = lua_gettop(State);

    // LuaHelper::StackTableReference ConsoleTable(State, 0, 2);

    lua_getglobal(State, "Assets");
    lua_getfield(State, -1, "MiscClassesGlobal");
    lua_getfield(State, -1, "BindableEvent");
    lua_getfield(State, -1, "new");
    lua_call(State, 0, 1);

    lua_setfield(State, TableIndex, "OnPrompt");
    lua_settop(State, TableIndex);
    lua_setglobal(State, CLibraries::Console::LibraryName);

    Game::Lua::CLibraries::Console::etc::_OnPromptThread =
        SDL_CreateThread(_OnPromptThreadFunc, "_OnPromptThread", NULL);
}

void Game::Lua::CLibraries::Console::Update(lua_State* State) {
    using namespace Game::Lua::CLibraries::Console::etc;
    if (_NewPrompt) {
        _NewPrompt = false;

        lua_getglobal(State, CLibraries::Console::LibraryName);
        lua_getfield(State, -1, "OnPrompt");
        lua_pushstring(State, _Prompt.c_str());
        lua_getfield(State, -1, "Fire");
        lua_call(State, 2, 0);

        lua_pop(State, 1);
    }
}

void Game::Lua::CLibraries::Console::DestroyLibrary() {
    using namespace Game::Lua::CLibraries::Console::etc;
	_RunThread = false;
    SDL_WaitThread(etc::_OnPromptThread, NULL);
}