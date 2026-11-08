#include "../define.h"

#ifdef BUILD_CLIENT

#include "LuaInput.hpp"

#include <cstring>
#include <iostream>
#include <vector>

#include "../../include/VM/lua.h"
#include "../../include/VM/lualib.h"

#include "../FunctionHeaders/LuaHelper.hpp"

#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>

#include "../Game.hpp"

int Game::Lua::CLibraries::Input::Binding::GetBindingName(lua_State* State) {
    lua_pushstring(State,
                   static_cast<Binding*>(
                       LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))
                       ->BindingName);
    return 1;
}

int Game::Lua::CLibraries::Input::Binding::Pressed(lua_State* State) {
    int Result = false;
    Uint8 i;

    std::vector<SDL_Scancode>& SDLScancodes =
        static_cast<Binding*>(
            LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))
            ->SDLScancodes;

    for (const auto& Scancode : SDLScancodes) {
        if (Scancode == SDL_SCANCODE_UNKNOWN) {
            continue;
        }

        if (Game::Window.ScancodePressed(Scancode)) {
            Result = true;
            break;
        }
    }

    lua_pushboolean(State, Result);
    return 1;
}

int Game::Lua::CLibraries::Input::Binding::Held(lua_State* State) {
    Uint8 i;

    std::vector<SDL_Scancode>& SDLScancodes =
        static_cast<Binding*>(
            LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))
            ->SDLScancodes;

    for (const SDL_Scancode Scancode : SDLScancodes) {
        if (Scancode > (int)SDL_SCANCODE_COUNT) {
            if (SDL_GetMouseState(NULL, NULL) &
                SDL_BUTTON_MASK(1 + Scancode - SDL_SCANCODE_COUNT)) {
                goto ReturnTrue;
            }
            continue;
        }

        if (Game::Window.ScancodeHeld(Scancode)) {
        ReturnTrue:
            lua_pushboolean(State, true);
            return 1;
        }
    }

    lua_pushboolean(State, false);
    return 1;
}

int Game::Lua::CLibraries::Input::Binding::Released(lua_State* State) {
    int Result = false;

    const std::vector<SDL_Scancode>& SDLScancodes =
        static_cast<Binding*>(
            LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))
            ->SDLScancodes;

    for (const auto& Scancode : SDLScancodes) {
        if (Scancode == SDL_SCANCODE_UNKNOWN) {
            continue;
        }

        if (Game::Window.ScancodeReleased(Scancode)) {
            Result = true;
            break;
        }
    }

    lua_pushboolean(State, Result);
    return 1;
}

int Game::Lua::CLibraries::Input::Binding::GetScancodes(lua_State* State) {
    int TableIndex = 0;

    const std::vector<SDL_Scancode>& SDLScancodes =
        static_cast<Binding*>(
            LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))
            ->SDLScancodes;

    lua_createtable(State, 0, SDLScancodes.size());
    for (const auto Scancode : SDLScancodes) {
        if (Scancode != SDL_SCANCODE_UNKNOWN) {
            // sub by one bc function doesn't account for pseudo indexes
            LuaHelper::SetIndex<lua_Integer>(State, -2 - 1,
                                             static_cast<lua_Integer>(Scancode),
                                             ++TableIndex);
        }
    }

    return 1;
}

int Game::Lua::CLibraries::Input::Binding::AddScancode(lua_State* State) {
    Uint8 ScancodeIndex = 0;
    std::vector<SDL_Scancode>& SDLScancodes =
        static_cast<Binding*>(
            LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))
            ->SDLScancodes;

    SDL_Scancode Scancode =
        static_cast<SDL_Scancode>(luaL_checkinteger(State, 2));

    while (true) {
        if (ScancodeIndex >= SDLScancodes.size()) {
            SDLScancodes.push_back(Scancode);
            break;
        }

        if (SDLScancodes[ScancodeIndex] == SDL_SCANCODE_UNKNOWN) {
            for (; ScancodeIndex < SDLScancodes.size(); ++ScancodeIndex) {
                unlikely_branch if (SDLScancodes[ScancodeIndex] == Scancode) {
                    std::cout << "Attempted to add a scancode: '"
                              << SDL_GetScancodeName(Scancode)
                              << "' to class "
                                 "'Binding' despite it already being "
                                 "added."
                              << std::endl;
                    return 0;
                }
            }

            SDLScancodes[ScancodeIndex] = Scancode;
            break;
        }
        ++ScancodeIndex;
    }
    return 0;
}

int Game::Lua::CLibraries::Input::Binding::RemoveScancode(lua_State* State) {
    Uint8 ScancodeIndex;
    SDL_Scancode RemoveScancode =
        static_cast<SDL_Scancode>(luaL_checkinteger(State, 2));

    std::vector<SDL_Scancode>& SDLScancodes =
        static_cast<Binding*>(
            LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))
            ->SDLScancodes;

    for (SDL_Scancode& Scancode : SDLScancodes) {
        if (Scancode == RemoveScancode) {
            *(&Scancode) = SDL_SCANCODE_UNKNOWN;
        }
    }
    return 0;
}

int Game::Lua::CLibraries::Input::Binding::ClearScancodes(lua_State* State) {
    static_cast<Binding*>(luaL_checkudata(State, 1, Binding::MetatableName))
        ->SDLScancodes.clear();
    return 0;
}

[[noreturn]]
int Game::Lua::CLibraries::Input::Binding::__newindex(lua_State* State) {
    luaL_error(State,
               "Attempted to write to a Binding object. Binding is "
               "read-only.");
}

int Game::Lua::CLibraries::Input::GetBinding(lua_State* State) {
    Input::Binding* BindingUD;
    size_t BindingNameLen;
    const char* BindingName;

    BindingName = luaL_checklstring(State, 1, &BindingNameLen);
    if (lua_getfield(State, lua_upvalueindex(1), BindingName) !=
        LUA_TUSERDATA) {
        BindingUD = static_cast<Input::Binding*>(
            lua_newuserdata(State, sizeof(Input::Binding) + BindingNameLen));

        unlikely_branch if (lua_gettop(State) < 3) {
            luaL_error(
                State,
                "Not enough arguments for 'Game.Input.GetBinding'. Need "
                "atleast 3 arguments to create a new binding. (only passed ",
                lua_gettop(State), "!)");
            return 0;
        }

        BindingUD->SDLScancodes = std::vector<SDL_Scancode>(
            lua_gettop(State) - 3, SDL_Scancode::SDL_SCANCODE_UNKNOWN);

        int i;
        for (i = 0; i < lua_gettop(State) - 3; ++i) {
            BindingUD->SDLScancodes[i] =
                static_cast<SDL_Scancode>(luaL_checkinteger(State, i + 2));
        } /*
         std::memset(
             &BindingUD->SDLScancodes[i], 0,
             sizeof(Input::Binding::SDLScancodes) - (sizeof(SDL_Scancode)) *
         i);*/
        std::memcpy(&BindingUD->BindingName, BindingName, BindingNameLen);

        luaL_getmetatable(State, CLibraries::Input::Binding::MetatableName);
        lua_setmetatable(State, -2);

        lua_pushvalue(State, -1);
        lua_setfield(State, lua_upvalueindex(1), BindingName);
    }
    /*
    std::cout << "stack top: " << luaL_typename(State, -1) << std::endl;
    lua_rotate(State, 1, 1);

    std::cout << "returning: " << luaL_typename(State, 1) << std::endl;*/
    return 1;
}
#endif