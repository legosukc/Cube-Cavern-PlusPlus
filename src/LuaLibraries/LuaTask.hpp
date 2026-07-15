#pragma once

#include <mutex>
#include <list>
#include <forward_list>

#include <lua.hpp>

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>

#include "../FunctionHeaders/LuaHelper.hpp"

namespace Game::Lua::CLibraries::Task {
	inline void Init(lua_State* State);
	inline void Update(lua_State* State);


	struct YieldInfo {
		lua_State* Thread;
		//std::chrono::steady_clock::time_point CreatedAt = std::chrono::steady_clock::now();
		//std::chrono::milliseconds WaitForMilliseconds;
		std::chrono::steady_clock::time_point ResumeAt;
		//volatile bool ReadyForExecution = false;
	};

	inline bool operator==(const YieldInfo& A, const YieldInfo& B) {
		return A.Thread == B.Thread;
	}

	std::list<YieldInfo> YieldedThreads;


	int CallUnderThread(lua_State* State, lua_State* Thread, int nargs = 0) {

		int nresults;
		switch (lua_resume(State, Thread, nargs, &nresults)) {
		case LUA_YIELD:
			break;
		}
		return nresults;
	}

	int CallUnderThread(lua_State* State, lua_CFunction Main) {

		lua_State* Thread = lua_newthread(State);
		LuaHelper::PushClosure(Thread, Main);
		return CallUnderThread(State, Thread, 1);
	}

	template<typename... Args_T>
	inline int CallUnderThread(lua_State* State, lua_State* Thread, Args_T... Args) {

		LuaHelper::PushArgs<Args_T...>(Thread, Args...);
		return CallUnderThread(State, Thread, sizeof...(Args));
	}

	template<typename... Args_T>
	inline int CallUnderThread(lua_State* State, lua_CFunction Main, Args_T... Args) {
		
		lua_State* Thread = lua_newthread(State);
		return CallUnderThread<lua_CFunction, Args_T...>(State, Thread, Main, Args...);
	}


	static int wait(lua_State* State);
	static int delay(lua_State* State);

	static int spawn(lua_State* State);
	static int defer(lua_State* State);

	static int cancel(lua_State* State);
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

			WaitFor = Thread.WaitForMilliseconds - MillisecondsWaited;
			if (WaitFor > std::chrono::milliseconds(0)) {


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

void Game::Lua::CLibraries::Task::Init(lua_State* State) {

	//::_ReadyThreads.

	//std::thread(::_TaskScheduler).detach();
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
			if (lua_resume(State, Thread.Thread, lua_gettop(Thread.Thread) - 1, NULL) != LUA_YIELD) {

				lua_close(Thread.Thread);
				Task::YieldedThreads.remove(Thread);
			}

			CurrentTimePoint = std::chrono::steady_clock::now();
		}
	}
}


static Game::Lua::CLibraries::Task::YieldInfo _CreateYieldTask(lua_State* State, lua_Number WaitForSeconds) {
	Game::Lua::CLibraries::Task::YieldInfo YieldInfo;
	YieldInfo.Thread = State;
	//YieldInfo.WaitForMilliseconds = std::chrono::milliseconds(static_cast<Uint64>(WaitForSeconds * 1000.0));
	YieldInfo.ResumeAt = std::chrono::steady_clock::now() + std::chrono::milliseconds(static_cast<Uint64>(WaitForSeconds * 1000.0)); //YieldInfo.CreatedAt + std::chrono::milliseconds(static_cast<Uint64>(WaitForSeconds * 1000.0));
	Game::Lua::CLibraries::Task::YieldedThreads.push_back(YieldInfo);
	return YieldInfo;
}


int Game::Lua::CLibraries::Task::wait(lua_State* State) {
	const std::chrono::steady_clock::time_point StartMillisecond = std::chrono::steady_clock::now();
	
	::_CreateYieldTask(State, luaL_optnumber(State, 1, 0.0));
	lua_settop(State, 0);

	lua_yield(State, 0);

	lua_pushnumber(State, static_cast<lua_Number>(
		std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - StartMillisecond).count()
		) / 1000.0
	);
	return 1;
}


int Continue(lua_State* State, int Status, lua_KContext ContinueContext) {


	return 1;
}

int Game::Lua::CLibraries::Task::delay(lua_State* State) {

	lua_State* NewThread;
	lua_Number DelayFor;

	luaL_checktype(State, 2, LUA_TFUNCTION);
	
	DelayFor = luaL_checknumber(State, 1);
	lua_rotate(State, 1, -1);
	lua_pop(State, 1);

	NewThread = lua_newthread(State);
	lua_rotate(State, 1, 1);

	lua_xmove(State, NewThread, lua_gettop(State) - 1);
	::_CreateYieldTask(NewThread, DelayFor);

	return 1;
}

int Game::Lua::CLibraries::Task::spawn(lua_State* State) {
	lua_State* NewThread;
	int StackTop;

	luaL_checktype(State, 1, LUA_TFUNCTION);

	NewThread = lua_newthread(State);
	StackTop = lua_gettop(State) - 1;

	lua_rotate(State, 1, 1);
	lua_xmove(State, NewThread, StackTop);

	::_CreateYieldTask(NewThread, 0.0);
	lua_resume(State, NewThread, StackTop, NULL);

	return 1;
}

int Game::Lua::CLibraries::Task::defer(lua_State* State) {

	lua_State* NewThread;

	luaL_checktype(State, 1, LUA_TFUNCTION);

	NewThread = lua_newthread(State);

	lua_rotate(State, 1, 1);
	lua_xmove(State, NewThread, lua_gettop(State) - 1);

	::_CreateYieldTask(NewThread, 0.0);

	return 1;
}

int Game::Lua::CLibraries::Task::cancel(lua_State* State) {

	lua_State* Thread = lua_tothread(State, 1);

	unlikely_branch
	if (Thread == NULL) {
		luaL_argerror(State, 1, "expected type 'thread'");
	}

	for (Task::YieldInfo& YieldedThreadInfo : Task::YieldedThreads) {

		if (YieldedThreadInfo.Thread == Thread) {
			Task::YieldedThreads.remove(YieldedThreadInfo);
			break;
		}
	}
	lua_closethread(State, Thread);

	return 0;
}