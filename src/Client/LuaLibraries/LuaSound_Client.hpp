#pragma once

#include "../../../include/VM/lua.h"
#include "../../../include/VM/lualib.h"

#include "../../FunctionHeaders/LuaHelper.hpp"

#include "../../MathClasses/Vector3.hpp"
#include "../Sound/Sound.hpp"

#include "../../Lua.hpp"


namespace Game::Lua::CLibraries::Sound {
	inline void Init(lua_State* State);

	static int SetListenerPosition(lua_State* State) {
		Game::Sound::Listener.SetPosition(*static_cast<Math::Vector3*>(luaL_checkudata(State, 1, "Vector3")));
		return 0;
	}

	static int SetListenerVelocity(lua_State* State) {
		Game::Sound::Listener.SetVelocity(*static_cast<Math::Vector3*>(luaL_checkudata(State, 1, "Vector3")));
		return 0;
	}

	static int SetListenerOrientation(lua_State* State) {
		Game::Sound::Listener.SetOrientation(
			*static_cast<Math::Vector3*>(luaL_checkudata(State, 1, "Vector3")),
			*static_cast<Math::Vector3*>(luaL_checkudata(State, 2, "Vector3"))
		);
		return 0;
	}


	static int CreateAudioSource(lua_State* State) {

		Game::Sound::Classes::AudioSource** AudioSourceUD = static_cast<Game::Sound::Classes::AudioSource**>(lua_newuserdata(State, sizeof(Game::Sound::Classes::AudioSource*)));
		*AudioSourceUD = Game::Sound::CreateAudioSource();

		luaL_getmetatable(State, "AudioSource");
		lua_setmetatable(State, -2);

		return 1;
	}

	static int CreateSpeaker(lua_State* State) {

		Game::Sound::Classes::Speaker** SpeakerUD = static_cast<Game::Sound::Classes::Speaker**>(lua_newuserdata(State, sizeof(Game::Sound::Classes::Speaker*)));
		*SpeakerUD = Game::Sound::CreateSpeaker();

		luaL_getmetatable(State, "Speaker");
		lua_setmetatable(State, -2);

		return 1;
	}

	struct AudioSourceMetatable {
		
		
	};

	struct SpeakerMetatable {

		static int Play(lua_State* State) {
			(*static_cast<Game::Sound::Classes::Speaker**>(luaL_checkudata(State, 1, "Speaker")))->Play();
			return 0;
		}
		static int Stop(lua_State* State) {
			(*static_cast<Game::Sound::Classes::Speaker**>(luaL_checkudata(State, 1, "Speaker")))->Stop();
			return 0;
		}
		static int Pause(lua_State* State) {
			(*static_cast<Game::Sound::Classes::Speaker**>(luaL_checkudata(State, 1, "Speaker")))->Pause();
			return 0;
		}
	};
}

void Game::Lua::CLibraries::Sound::Init(lua_State* State) {

	LuaHelper::StackTableReference AudioSourceMetatable(State, "AudioSource");

	AudioSourceMetatable.SetKeyClosure(State, CLibraries::Sound::SetListenerPosition, "SetListenerPosition");


	LuaHelper::StackTableReference SpeakerMetatable(State, "Speaker");

	SpeakerMetatable.SetKeyClosure(State, CLibraries::Sound::SpeakerMetatable::Play, "Play");
	SpeakerMetatable.SetKeyClosure(State, CLibraries::Sound::SpeakerMetatable::Pause, "Pause");
	SpeakerMetatable.SetKeyClosure(State, CLibraries::Sound::SpeakerMetatable::Stop, "Stop");

	lua_settop(State, AudioSourceMetatable.GetStackIndex() - 1);


	lua_createtable(State, 0, 5);

	LuaHelper::SetKeyClosure(State, -2, CLibraries::Sound::SetListenerPosition, "SetListenerPosition");
	LuaHelper::SetKeyClosure(State, -2, CLibraries::Sound::SetListenerVelocity, "SetListenerVelocity");
	LuaHelper::SetKeyClosure(State, -2, CLibraries::Sound::SetListenerOrientation, "SetListenerOrientation");

	LuaHelper::SetKeyClosure(State, -2, CLibraries::Sound::CreateAudioSource, "CreateAudioSource");
	LuaHelper::SetKeyClosure(State, -2, CLibraries::Sound::CreateSpeaker, "CreateSpeaker");

	lua_setfield(State, Game::Lua::GameTable.GetStackIndex(), "Sound");
}