#ifndef LUALIBRARIES_LUAENUMS_HPP
#define LUALIBRARIES_LUAENUMS_HPP

#include "../define.h"

#include "../../include/VM/lua.h"

#include <SDL3/SDL_stdinc.h>

namespace Game::Lua::CLibraries::Enums {

    void Init(lua_State* State);

    inline constexpr const char* const LibraryName = "Enums";

    namespace Classes {
        struct EnumValue {
            static INLINE_MEMBER_VARIABLE const char* const MetatableName = "Enum";
			
			int Uservalue;
            int ID;

            static int __index(lua_State* State);
        };
    }
    
    int FromID(lua_State* State);

    bool RegisterEnumTable(lua_State* State,
                           int EnumTableStackIndex,
                           const char* EnumTableName);

    Classes::EnumValue* PushNewEnum(lua_State* State, int Uservalue = 0);
}
#endif