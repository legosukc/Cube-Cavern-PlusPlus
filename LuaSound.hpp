
namespace Game::Lua::CLibraries::Sound {
	inline void Init();

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


	struct Buffer {
		static inline void InitMetatable(lua_State* State);

		Game::Sound::Buffer CppBuffer;
	};
}