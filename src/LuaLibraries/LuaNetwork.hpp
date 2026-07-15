
namespace Game::Lua::CLibraries::Network {
	inline void Init(lua_State* State);
	inline void Destroy(lua_State* State);

	static int GetRemotePool(lua_State* State) {

		

		return 1;
	}

	struct RemoteEvent {
		static inline void InitMetatable(lua_State* State);
	};
}

void Game::Lua::CLibraries::Network::Init(lua_State* State) {
	
	LuaHelper::StackTableReference PacketsTable(State, 0, 1);

	lua_getglobal(State, "Game");
	lua_pushvalue(State, PacketsTable.GetStackIndex());
	lua_setfield(State, PacketsTable.GetStackIndex() + 1, "Network");


}