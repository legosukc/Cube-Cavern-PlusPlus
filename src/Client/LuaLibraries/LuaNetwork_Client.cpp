#include "LuaNetwork_Client.hpp"

#include <cstring>
#include <vector>

#include "../../../include/VM/lua.h"
#include "../../FunctionHeaders/LuaHelper.hpp"

int Game::Lua::CLibraries::Network::GetRemotePool(lua_State* State) {
    return 1;
}

// TODO: implement fully
int Game::Lua::CLibraries::Network::RemoteEvent::Fire(lua_State* State) {
    RemoteEvent* RemoteEventUD;
    const char* Typename;
    int StackTop;

    RemoteEventUD = static_cast<RemoteEvent*>(
        LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)));
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
            // std::find(UsedTypenames.begin(), UsedTypenames.end(),
            // Typename);
            while (true) {
                if (std::strcmp(UsedTypenames[i], Typename)) {
                    // EncodedData.
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