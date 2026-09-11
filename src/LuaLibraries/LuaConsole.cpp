#include "LuaConsole.hpp"

#include <iostream>
#include <string>

#include "../../include/VM/lua.h"

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_thread.h>

void Game::Lua::CLibraries::Console::Init(lua_State* State) {
    lua_createtable(State, 0, 8);
    lua_setfield(State, -2, "");
}

static SDL_Thread* _OnPromptThread;

static volatile bool _RunThread = true;
static volatile bool _NewPrompt = false;
static std::string _Prompt;

static int _OnPromptThreadFunc(void*) {
    if (!SDL_SetCurrentThreadPriority(
            SDL_ThreadPriority::SDL_THREAD_PRIORITY_LOW)) {
        std::clog << "Failed to set thread priority for _OnPromptThreadFunc."
                  << std::endl;
    }

    while (_RunThread) {
        std::getline(std::cin, _Prompt);
        _NewPrompt = true;
    }

    return -1;
}

void Game::Lua::CLibraries::Console::PostMiscClassInit(lua_State* State) {

    lua_createtable(State, 0, 2);
    const int TableIndex = lua_gettop(State);

    //LuaHelper::StackTableReference ConsoleTable(State, 0, 2);

    lua_getglobal(State, "Assets");
    lua_getfield(State, -1, "MiscClassesGlobal");
    lua_getfield(State, -1, "BindableEvent");
    lua_getfield(State, -1, "new");
    lua_call(State, 0, 1);

    lua_setfield(State, TableIndex, "OnPrompt");
    lua_settop(State, TableIndex);
    lua_setglobal(State, CLibraries::Console::LibraryName);

    _OnPromptThread =
        SDL_CreateThread(_OnPromptThreadFunc, "_OnPromptThread", NULL);
}

void Game::Lua::CLibraries::Console::Update(lua_State* State) {
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
    _RunThread = false;
    SDL_WaitThread(_OnPromptThread, NULL);
}