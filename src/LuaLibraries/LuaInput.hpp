#pragma once

#include "../define.h"

#include "../../include/VM/lua.h"

#include "../FunctionHeaders/LuaHelper.hpp"

#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>

#include <vector>

#include "../MathClasses/Vector2.hpp"

#include "../Game.hpp"
//#include "../Lua.hpp"

namespace Game::Lua {
    extern LuaHelper::StackTableReference GameTable;
}

namespace Game::Lua::CLibraries::Input {

    inline void Init(lua_State* State);

#ifdef BUILD_CLIENT
    inline void Update(lua_State* State);

    struct Binding {
        static constexpr char MetatableName[] = "Binding";

        std::vector<SDL_Scancode> SDLScancodes;
        char BindingName[];

        static int GetBindingName(lua_State* State);

        static int Pressed(lua_State* State);
        static int Held(lua_State* State);
        static int Released(lua_State* State);

        static int GetScancodes(lua_State* State);

        static int AddScancode(lua_State* State);
        static int RemoveScancode(lua_State* State);
        static int ClearScancodes(lua_State* State);

        [[noreturn]]
        static int __newindex(lua_State* State);
    };

    int GetBinding(lua_State* State);

    static const char* const LibraryName = "Input";
#endif
}

void Game::Lua::CLibraries::Input::Init(lua_State* State) {
    LuaHelper::StackTableReference EnumScancode, BindingsTable,
        BindingMetatable;

    lua_getglobal(State, "Enums");
    lua_pushliteral(State, "Scancode");

    EnumScancode = LuaHelper::StackTableReference(
        State, 0, static_cast<int>(SDL_Scancode::SDL_SCANCODE_COUNT));

    struct ScancodeEnumStruct {
        const char* Name;
        SDL_Scancode Scancode;
    };

    constexpr static ScancodeEnumStruct ScancodeEnums[] = {

        {"One", SDL_SCANCODE_1},
        {"Two", SDL_SCANCODE_2},
        {"Three", SDL_SCANCODE_3},
        {"Four", SDL_SCANCODE_4},
        {"Five", SDL_SCANCODE_5},
        {"Six", SDL_SCANCODE_6},
        {"Seven", SDL_SCANCODE_7},
        {"Eight", SDL_SCANCODE_8},
        {"Nine", SDL_SCANCODE_9},
        {"Zero", SDL_SCANCODE_0},
        {"Minus", SDL_SCANCODE_MINUS},
        {"Equals", SDL_SCANCODE_EQUALS},
        {"Backspace", SDL_SCANCODE_BACKSPACE},

        {"Q", SDL_SCANCODE_Q},
        {"W", SDL_SCANCODE_W},
        {"E", SDL_SCANCODE_E},
        {"R", SDL_SCANCODE_R},
        {"T", SDL_SCANCODE_T},
        {"Y", SDL_SCANCODE_Y},
        {"U", SDL_SCANCODE_U},
        {"I", SDL_SCANCODE_I},
        {"O", SDL_SCANCODE_O},
        {"LeftBracket", SDL_SCANCODE_LEFTBRACKET},
        {"RightBracket", SDL_SCANCODE_RIGHTBRACKET},
        {"Backslash", SDL_SCANCODE_BACKSLASH},

        {"W", SDL_SCANCODE_W},
        {"A", SDL_SCANCODE_A},
        {"S", SDL_SCANCODE_S},
        {"D", SDL_SCANCODE_D},

        {"Z", SDL_SCANCODE_Z},
        {"X", SDL_SCANCODE_X},
        {"C", SDL_SCANCODE_C},
        {"V", SDL_SCANCODE_V},

        {"Enter", SDL_SCANCODE_RETURN},
        {"Tab", SDL_SCANCODE_TAB},

        {"Space", SDL_SCANCODE_SPACE},

        {"LeftShift", SDL_SCANCODE_LSHIFT},
        {"RightShift", SDL_SCANCODE_RSHIFT},

        {"LeftCtrl", SDL_SCANCODE_LCTRL},
        {"RightCtrl", SDL_SCANCODE_RCTRL},

        {"LeftAlt", SDL_SCANCODE_LALT},
        {"RightAlt", SDL_SCANCODE_RALT},

        {"LeftClick", static_cast<SDL_Scancode>((int)SDL_SCANCODE_COUNT + 1)},
        {"RightClick", static_cast<SDL_Scancode>((int)SDL_SCANCODE_COUNT + 2)},
    };

    for (const ScancodeEnumStruct& ScancodeEnum : ScancodeEnums) {
        EnumScancode.SetKey<lua_Integer>(
            State, static_cast<lua_Integer>(ScancodeEnum.Scancode),
            ScancodeEnum.Name);
    }

    lua_rawset(State, EnumScancode.GetStackIndex() - 2);

#ifdef BUILD_CLIENT
    BindingMetatable = LuaHelper::StackTableReference(
        State, CLibraries::Input::Binding::MetatableName);

    for (Uint8 i = 0; i < 7; ++i) {
        BindingMetatable.PushReference(State);
    }

    BindingMetatable.SetKeyClosure(State, Binding::GetBindingName,
                                   "GetBindingName", 1);

    BindingMetatable.SetKeyClosure(State, Binding::Pressed, "Pressed", 1);
    BindingMetatable.SetKeyClosure(State, Binding::Held, "Held", 1);
    BindingMetatable.SetKeyClosure(State, Binding::Released, "Released", 1);

    BindingMetatable.SetKeyClosure(State, Binding::GetScancodes, "GetScancodes",
                                   1);
    BindingMetatable.SetKeyClosure(State, Binding::AddScancode, "AddScancode",
                                   1);

    BindingMetatable.SetKeyClosure(State, Binding::RemoveScancode,
                                   "RemoveScancode", 1);
    BindingMetatable.SetKeyClosure(State, Binding::ClearScancodes,
                                   "ClearScancodes");

    BindingMetatable.SetKeyClosure(State, Binding::__newindex, "__newindex");
    // BindingMetatable.SetKey<bool>(State, false, "__metatable");

    BindingMetatable.PushReference(State);
    lua_setfield(State, BindingMetatable.GetStackIndex(), "__index");
    lua_settop(State, BindingMetatable.GetStackIndex() - 1);

    // Bindings Table
    BindingsTable = LuaHelper::StackTableReference(State, 0, 2);

    // std::memset(lua_newuserdata(State, sizeof(Math::Vector2)), 0,
    // sizeof(Math::Vector2)); luaL_setmetatable(State, "Vector2");

    BindingsTable.SetKey<lua_Number>(
        State, static_cast<lua_Number>(Game::Window.MouseDelta.X),
        "MouseDeltaX");

    BindingsTable.SetKey<lua_Number>(
        State, static_cast<lua_Number>(Game::Window.MouseDelta.Y),
        "MouseDeltaY");

    // TODO: get mouse pos
    BindingsTable.SetKey<lua_Number>(
        State, static_cast<lua_Number>(Game::Window.MouseDelta.X),
        "MouseRelativePositionX");

    BindingsTable.SetKey<lua_Number>(
        State, static_cast<lua_Number>(Game::Window.MouseDelta.Y),
        "MouseRelativePositionY");

    // std::memset(lua_newuserdata(State, sizeof(Math::Vector2)), 0,
    // sizeof(Math::Vector2)); luaL_setmetatable(State, "Vector2");

    lua_createtable(State, 0, 8);
    BindingsTable.SetKeyClosure(State, Input::GetBinding, "GetBinding", 1);

    LuaHelper::LockTable(State, BindingsTable.GetStackIndex());

    lua_setfield(State, Game::Lua::GameTable.GetStackIndex(),
                 CLibraries::Input::LibraryName);
#endif
}

#ifdef BUILD_CLIENT

void Game::Lua::CLibraries::Input::Update(lua_State* State) {
    int InputTableIndex;
    Math::Vector2* VectorUD;

    Game::Lua::GameTable.PushKey(State, CLibraries::Input::LibraryName);
    InputTableIndex = lua_gettop(State);

    lua_pushliteral(State, "MouseDeltaX");
    lua_pushnumber(State, static_cast<lua_Number>(Game::Window.MouseDelta.X));
    lua_rawset(State, InputTableIndex);

    lua_pushliteral(State, "MouseDeltaY");
    lua_pushnumber(State, static_cast<lua_Number>(Game::Window.MouseDelta.Y));
    lua_rawset(State, InputTableIndex);

    // TODO: get mouse pos
    lua_pushliteral(State, "MouseRelativePositionX");
    lua_pushnumber(State, static_cast<lua_Number>(Game::Window.MouseDelta.X));
    lua_rawset(State, InputTableIndex);

    lua_pushliteral(State, "MouseRelativePositionY");
    lua_pushnumber(State, static_cast<lua_Number>(Game::Window.MouseDelta.Y));
    lua_rawset(State, InputTableIndex);

    lua_settop(State, InputTableIndex - 1);
}
#endif