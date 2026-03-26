
namespace Game::Lua::CLibraries::Graphics::Classes {

	struct GLObjectBase {
		static void InitMetatable(lua_State* State, const char* MetatableName);

		GLuint GLObject;

		static int __gc(lua_State* State);
		static int __eq(lua_State* State);
	};
}


void Game::Lua::CLibraries::Graphics::Classes::GLObjectBase::InitMetatable(lua_State* State, const char* MetatableName) {

	LuaHelper::StackTableReference Metatable(State, MetatableName);

	Metatable.SetKey(State, GLObjectBase::__gc, "__gc");
	Metatable.SetKey(State, GLObjectBase::__eq, "__eq");
}

int Game::Lua::CLibraries::Graphics::Classes::GLObjectBase::__gc(lua_State* State) {
	return 0;
}

int Game::Lua::CLibraries::Graphics::Classes::GLObjectBase::__eq(lua_State* State) {

	lua_getmetatable(State, 1);
	lua_getmetatable(State, 2);

	bool Result;
	if (lua_rawequal(State, -1, -2)) {
		Result = static_cast<GLObjectBase*>(lua_touserdata(State, 1))->GLObject == static_cast<GLObjectBase*>(lua_touserdata(State, 2))->GLObject;
	} else {
		Result = false;
	}
	lua_settop(State, 0);
	lua_pushboolean(State, static_cast<int>(Result));
	return 1;
}