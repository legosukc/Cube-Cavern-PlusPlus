#pragma once

#include <vector>
#include <algorithm>
#include <cstring>

#include "../../../include/VM/lua.h"
#include "../../FunctionHeaders/LuaHelper.hpp"


namespace Game::Lua::CLibraries::Network {
	inline void Init(lua_State* State);
	inline void Destroy(lua_State* State);
	
	enum class LuaTypeMappings {
		Nil,
		Integer, Number, Boolean,
		String,
		Function,
		Table, Buffer,
		Vector2, IVector2, UVector2,
		Vector3, IVector3, UVector3,
		Vector4, IVector4, UVector4,
		Enum,
	};

	static int GetRemotePool(lua_State* State) {

		

		return 1;
	}

	struct RemoteEvent {
		static inline void InitMetatable(lua_State* State);

		// TODO: implement fully
		static int Fire(lua_State* State) {

			RemoteEvent* RemoteEventUD;
			const char* Typename;
			int StackTop;

			RemoteEventUD = static_cast<RemoteEvent*>(LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)));
			StackTop = lua_gettop(State);
			
			std::vector<const char*> UsedTypenames;
			std::vector<char> EncodedData;

			int UsedTypenameIndex = 0;
			size_t BufferSize = 0;
			for (int i = 2; i <= StackTop; ++i) {

				if (lua_isuserdata(State, i)) {

					void* Userdata = lua_touserdata(State, i);
					
					lua_getmetatable(State, i);
					if (lua_getfield(State, -1, "__name") == LUA_TSTRING) {
						Typename = lua_tostring(State, -1);
					} else {
						Typename = "userdata";
					}
					lua_settop(State, StackTop);
					//std::find(UsedTypenames.begin(), UsedTypenames.end(), Typename);
					while (true) {
						
						if (std::strcmp(UsedTypenames[i], Typename)) {
							//EncodedData.
							break;
						}

						if (++UsedTypenameIndex > UsedTypenames.size()) {
							break;
						}
					}
				}
			}
			return 0;
		}
	};
}

void Game::Lua::CLibraries::Network::Init(lua_State* State) {
	
	LuaHelper::StackTableReference PacketsTable(State, 0, 1);

	lua_getglobal(State, "Game");
	lua_pushvalue(State, PacketsTable.GetStackIndex());
	lua_setfield(State,  PacketsTable.GetStackIndex() + 1, "Network");


}