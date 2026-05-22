#pragma once

#include <lua-5.5.0/lua.hpp>


#include "../../MathClasses/Vector3.hpp"
#include "../Sound_Client.hpp"


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


	struct SoundBuffer {
		static inline void InitMetatable(lua_State* State);

		Game::Sound::Buffer CppBuffer;

		static int UploadSoundData(lua_State* State) {
			using DataBuffer = Game::Lua::CLibraries::Buffer::Classes::Buffer;

			const DataBuffer* DataBufferUD = static_cast<DataBuffer*>(luaL_checkudata(State, 2, DataBuffer::MetatableName));
			
			static_cast<SoundBuffer*>(lua_touserdata(State, 1))->CppBuffer.UploadSoundData(
				DataBufferUD->Data,
				DataBufferUD->Size,
				static_cast<ALenum>(luaL_checkinteger(State, 3)),
				static_cast<ALsizei>(luaL_checkinteger(State, 4))
			);
			return 0;
		}

		static int LoadFile(lua_State* State) {

			lua_pushboolean(State,
				static_cast<SoundBuffer*>(lua_touserdata(State, 1))->CppBuffer.LoadFile(
					luaL_checkstring(State, 2),
					static_cast<ALenum>(luaL_checkinteger(State, 3))
				)
			);
			return 1;
		}
	};
}

void Game::Lua::CLibraries::Sound::Init(lua_State* State) {

	
}