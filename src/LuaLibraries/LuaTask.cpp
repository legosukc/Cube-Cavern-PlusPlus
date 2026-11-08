#include "LuaTask.hpp"

// TODO: fix this for Luau

#include <list>

#include "../../include/VM/lua.h"
#include "../../include/VM/lualib.h"

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>

#include "../FunctionHeaders/LuaHelper.hpp"

namespace Game::Lua::CLibraries::Task {
    std::list<YieldInfo> YieldedThreads;
}

int Game::Lua::CLibraries::Task::CallUnderThread(lua_State* State,
                                                 lua_State* Thread,
                                                 int nargs) {
    return lua_resume(State, Thread, nargs);
}

int Game::Lua::CLibraries::Task::CallUnderThread(lua_State* State,
                                                 lua_CFunction Main) {
    lua_State* Thread = lua_newthread(State);
    LuaHelper::PushClosure(Thread, Main, "main");
    return CallUnderThread(State, Thread, 1);
}

/*
static void _TaskScheduler() {

        std::chrono::milliseconds MillisecondsWaited, WaitFor;
        while (true) {

                if (::_YieldedThreads.size() == 0) {
                        std::this_thread::sleep_for(std::chrono::microseconds(500));
                        continue;
                }

                MillisecondsWaited = std::chrono::milliseconds(0);

                ::_YieldedThreadsMutex.lock();
                ::_ReadyThreadsMutex.lock();

                for (auto& Thread : ::_YieldedThreads) {

                        WaitFor = Thread.WaitForMilliseconds -
MillisecondsWaited; if (WaitFor > std::chrono::milliseconds(0)) {


                                std::this_thread::sleep_for(std::chrono::milliseconds(WaitFor));


                                MillisecondsWaited += WaitFor;
                        }

                        ::_ReadyThreads.push_front(Thread.Thread);
                        ::_YieldedThreadsMutex.unlock();

                        //Thread.ReadyForExecution = true;
                }
                ::_YieldedThreads.clear();

                ::_YieldedThreadsMutex.unlock();
                ::_ReadyThreadsMutex.unlock();
        }
}*/

static Game::Lua::CLibraries::Task::YieldInfo _CreateYieldTask(
    lua_State* Thread,
    lua_Number WaitForSeconds) {
    Game::Lua::CLibraries::Task::YieldInfo YieldInfo;
    YieldInfo.Thread = Thread;
    // YieldInfo.WaitForMilliseconds =
    // std::chrono::milliseconds(static_cast<Uint64>(WaitForSeconds * 1000.0));
    YieldInfo.ResumeAt =
        std::chrono::steady_clock::now() +
        std::chrono::milliseconds(static_cast<Uint64>(
            WaitForSeconds *
            1000.0));  // YieldInfo.CreatedAt +
                       // std::chrono::milliseconds(static_cast<Uint64>(WaitForSeconds
                       // * 1000.0));
    Game::Lua::CLibraries::Task::YieldedThreads.push_back(YieldInfo);
    return YieldInfo;
}

int Game::Lua::CLibraries::Task::wait(lua_State* State) {
    const std::chrono::steady_clock::time_point StartMillisecond =
        std::chrono::steady_clock::now();

    ::_CreateYieldTask(State, luaL_optnumber(State, 1, 0.0));
    lua_settop(State, 0);

    lua_yield(State, 0);

    lua_pushnumber(State,
                   static_cast<lua_Number>(
                       std::chrono::duration_cast<std::chrono::milliseconds>(
                           std::chrono::steady_clock::now() - StartMillisecond)
                           .count()) /
                       1000.0);
    return 1;
}
/*
int Continue(lua_State* State, int Status, lua_KContext ContinueContext) {
    return 1;
}*/

static int ____delay(lua_State* State) {
    lua_pushcfunction(State, Game::Lua::CLibraries::Task::wait, "task.wait");
    lua_insert(State, 1);
    lua_call(State, 1, 0);
    lua_call(State, lua_gettop(State) - 1, 0);
    return 0;
}

int Game::Lua::CLibraries::Task::delay(lua_State* State) {
    lua_State* NewThread;
    lua_Number DelayFor;

    DelayFor = luaL_checknumber(State, 1);
    luaL_checktype(State, 2, LUA_TFUNCTION);

    NewThread = lua_newthread(State);
    lua_insert(State, 1);

    // lua_xmove(State, NewThread, lua_gettop(State) - 1);
    // lua_resume(State, lua_State *from, int narg)
    //::_CreateYieldTask(NewThread, DelayFor);

    return 1;
}

int Game::Lua::CLibraries::Task::spawn(lua_State* State) {
    lua_State* NewThread;
    int StackTop;

    luaL_checktype(State, 1, LUA_TFUNCTION);

    NewThread = lua_newthread(State);
    StackTop = lua_gettop(State) - 1;

    lua_insert(State, 1);
    lua_xmove(State, NewThread, StackTop);

    ::_CreateYieldTask(NewThread, 0.0);
    lua_resume(State, NewThread, StackTop);

    return 1;
}

int Game::Lua::CLibraries::Task::defer(lua_State* State) {
    lua_State* NewThread;

    luaL_checktype(State, 1, LUA_TFUNCTION);

    NewThread = lua_newthread(State);

    lua_insert(State, 1);
    lua_xmove(State, NewThread, lua_gettop(State) - 1);

    ::_CreateYieldTask(NewThread, 0.0);

    return 1;
}

int Game::Lua::CLibraries::Task::cancel(lua_State* State) {
    lua_State* Thread = lua_tothread(State, 1);

    unlikely_branch if (Thread == NULL) {
        luaL_argerror(State, 1, "expected type 'thread'");
    }

    for (Task::YieldInfo& YieldedThreadInfo : Task::YieldedThreads) {
        if (YieldedThreadInfo.Thread == Thread) {
            Task::YieldedThreads.remove(YieldedThreadInfo);
            break;
        }
    }

    // lua_closethread(State, Thread);

    return 0;
}