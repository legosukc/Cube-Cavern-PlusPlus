
namespace Game::Lua::CLibraries::Input {

	inline void Init(lua_State* State);

	static int Update(lua_State* State);


	struct Binding {
		static inline void InitMetatable(lua_State* State);

		Uint16 SDLScancodes[16];
		char BindingName[];


		static int GetBindingName(lua_State* State);

		static int GetKeycodes(lua_State* State);
		static int AddKeycode(lua_State* State);
		static int RemoveKeycode(lua_State* State);
		static int ClearKeycodes(lua_State* State);

		static int __eq(lua_State* State);
		static int __newindex(lua_State* State);
	};

	static int GetBinding(lua_State* State);
}


void Game::Lua::CLibraries::Input::Init(lua_State* State) {

	Binding::InitMetatable(State);


	// Bindings Table
	lua_createtable(State, 0, 16);

	lua_pushcclosure(State, Input::GetBinding, 1);
	lua_setfield(State, -2, "GetBinding");

	lua_setglobal(State, "Input");
}


void Game::Lua::CLibraries::Input::Binding::InitMetatable(lua_State* State) {

	luaL_newmetatable(State, "Binding");

	lua_pushboolean(State, static_cast<int>(false));
	lua_setfield(State, -2, "Pressed");

	lua_pushboolean(State, static_cast<int>(false));
	lua_setfield(State, -2, "Held");


	lua_pushcfunction(State, Binding::__newindex);
	lua_setfield(State, -2, "__newindex");

	lua_pushcfunction(State, Binding::__eq);
	lua_setfield(State, -2, "__eq");

	lua_pushboolean(State, static_cast<int>(false));
	lua_setfield(State, -2, "__metatable");

	lua_pop(State, 1);
}


int Game::Lua::CLibraries::Input::GetBinding(lua_State* State) {

	lua_settop(State, 1);

	size_t BindingNameLen;
	const char* BindingName = luaL_checklstring(State, 1, &BindingNameLen);
	if (lua_gettable(State, lua_upvalueindex(1)) == LUA_TNIL) {

		Input::Binding* const BindingUD = reinterpret_cast<Input::Binding*>(lua_newuserdata(State, sizeof(Input::Binding) + BindingNameLen));
		std::memset(&BindingUD->SDLScancodes, 0, sizeof(Input::Binding::SDLScancodes));
		std::memcpy(&BindingUD->BindingName, BindingName, BindingNameLen);

		luaL_setmetatable(State, "Binding");

		lua_pushvalue(State, 1);
		lua_pushvalue(State, -1);

		lua_settable(State, lua_upvalueindex(1));
	}

	lua_rotate(State, 0, 1);
	return 1;
}