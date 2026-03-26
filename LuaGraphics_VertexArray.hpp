
namespace Game::Lua::CLibraries::Graphics {

	namespace Classes {
		struct VertexArray : Game::Lua::CLibraries::Graphics::Classes::GLObjectBase {
			static inline void InitMetatable(lua_State* State);

			static int Bind(lua_State* State);
			static int __gc(lua_State* State);
		};
	}

	namespace VertexArray {
		static inline void Init(lua_State* State);

		static int Create(lua_State* State);

		static int Unbind(lua_State* State);
	}
}


void Game::Lua::CLibraries::Graphics::VertexArray::Init(lua_State* State) {

	LuaHelper::StackTableReference VertexArrayTable(State, 0, 2); // OpenGL.VertexArray

	VertexArrayTable.SetKey(State, VertexArray::Create, "Create");
	VertexArrayTable.SetKey(State, VertexArray::Unbind, "Unbind");

	lua_setfield(State, VertexArrayTable.TableIndex - 1, "VertexArray");
}

int Game::Lua::CLibraries::Graphics::VertexArray::Create(lua_State* State) {
	Classes::VertexArray* VertexArrayUD = static_cast<Classes::VertexArray*>(lua_newuserdata(State, sizeof(Classes::VertexArray)));
	glGenVertexArrays(1, &VertexArrayUD->GLObject);
	luaL_setmetatable(State, "VertexArray");
	return 1;
}

int Game::Lua::CLibraries::Graphics::VertexArray::Unbind(lua_State* State) {
	glBindVertexArray(0);
	return 0;
}


void Game::Lua::CLibraries::Graphics::Classes::VertexArray::InitMetatable(lua_State* State) {
	GLObjectBase::InitMetatable(State, "VertexArray");

	LuaHelper::SetKey(State, -2, VertexArray::Bind, "Bind");
}

int Game::Lua::CLibraries::Graphics::Classes::VertexArray::Bind(lua_State* State) {
	glBindVertexArray(static_cast<VertexArray*>(lua_touserdata(State, 1))->GLObject);
	return 0;
}

int Game::Lua::CLibraries::Graphics::Classes::VertexArray::__gc(lua_State* State) {
	glDeleteVertexArrays(1, &static_cast<VertexArray*>(lua_touserdata(State, 1))->GLObject);
	return 0;
}
