#pragma once

#include "../../../../include/VM/lua.h"

namespace Game::Lua::CLibraries::RenderObjectImplementations {
    class CRenderObject {
       public:
        static inline void Init(lua_State* State) {}
        static inline void Destroy(lua_State* State) {}

        static int Draw(lua_State* State);
    };
}