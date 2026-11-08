#pragma once

#include <list>
#include <chrono>

#include "../../include/VM/lua.h"

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>

#include "../FunctionHeaders/LuaHelper.hpp"

namespace Game::Lua::CLibraries::Task {
    inline void Init(lua_State* State);
    inline void Update(lua_State* State);

    struct YieldInfo {
        lua_State* Thread;
        // std::chrono::steady_clock::time_point CreatedAt =
        // std::chrono::steady_clock::now(); std::chrono::milliseconds
        // WaitForMilliseconds;
        std::chrono::steady_clock::time_point ResumeAt;
        // volatile bool ReadyForExecution = false;
    };

    inline bool operator==(const YieldInfo& A, const YieldInfo& B) {
        return A.Thread == B.Thread;
    }

    extern std::list<YieldInfo> YieldedThreads;

    int CallUnderThread(lua_State* State, lua_State* Thread, int nargs = 0);

    int CallUnderThread(lua_State* State, lua_CFunction Main);

    template <typename... Args_T>
    inline int CallUnderThread(lua_State* State,
                               lua_State* Thread,
                               Args_T... Args) {
        LuaHelper::PushArgs<Args_T...>(Thread, Args...);
        return CallUnderThread(State, Thread, sizeof...(Args));
    }

    template <typename... Args_T>
    inline int CallUnderThread(lua_State* State,
                               lua_CFunction Main,
                               Args_T... Args) {
        lua_State* Thread = lua_newthread(State);
        return CallUnderThread<lua_CFunction, Args_T...>(State, Thread, Main,
                                                         Args...);
    }

    int wait(lua_State* State);
    int delay(lua_State* State);

    int spawn(lua_State* State);
    int defer(lua_State* State);

    int cancel(lua_State* State);
}

void Game::Lua::CLibraries::Task::Init(lua_State* State) {
    //::_ReadyThreads.

    // std::thread(::_TaskScheduler).detach();
    LuaHelper::SetGlobal(State, Task::wait, "wait");
    LuaHelper::SetGlobal(State, Task::delay, "delay");

    LuaHelper::SetGlobal(State, Task::spawn, "spawn");
    LuaHelper::SetGlobal(State, Task::defer, "defer");

    LuaHelper::SetGlobal(State, Task::cancel, "cancel");
}

void Game::Lua::CLibraries::Task::Update(lua_State* State) {
    /*
    for (lua_State* Thread : ::_ReadyThreads) {
            lua_resume(State, Thread, lua_gettop(Thread) - 1, NULL);
    }
    ::_ReadyThreads.clear();*/

    auto CurrentTimePoint = std::chrono::steady_clock::now();
    for (auto& Thread : Task::YieldedThreads) {
        if (CurrentTimePoint >= Thread.ResumeAt) {
            if (lua_resume(State, Thread.Thread,
                           lua_gettop(Thread.Thread) - 1) != LUA_YIELD) {
                lua_close(Thread.Thread);
                Task::YieldedThreads.remove(Thread);
            }

            CurrentTimePoint = std::chrono::steady_clock::now();
        }
    }
}