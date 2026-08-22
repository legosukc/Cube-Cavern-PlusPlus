#pragma once

#include "../define.h"

#include "../../include/VM/lua.h"
#include "../../include/VM/lualib.h"

#include "../FunctionHeaders/LuaHelper.hpp"

#include <algorithm>
#include <iostream>

#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_stdinc.h>

#include <cstring>
#include <vector>

#include "LuaVector.hpp"

#include "../MathClasses/Vector2.hpp"

#include "../Game.hpp"
#include "../Lua.hpp"

namespace Game::Lua::CLibraries::Input {

    inline void Init(lua_State* State);

#ifndef BUILD_SERVER
    inline void Update(lua_State* State);
#endif

    static inline const char* const LibraryName = "Input";

#ifndef BUILD_SERVER
    struct Binding {
        static inline const char* const MetatableName = "Binding";

        std::vector<SDL_Scancode> SDLScancodes;
        char BindingName[];

        static int GetBindingName(lua_State* State) {
            lua_pushstring(
                State, static_cast<Binding*>(LuaHelper::CheckMetatable(
                                                 State, 1, lua_upvalueindex(1)))
                           ->BindingName);
            return 1;
        }

        static int Pressed(lua_State* State) {
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

        static int Held(lua_State* State) {
            Uint8 i;

            std::vector<SDL_Scancode>& SDLScancodes =
                static_cast<Binding*>(
                    LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))
                    ->SDLScancodes;

            for (const SDL_Scancode Scancode : SDLScancodes) {
                switch (Scancode) {
                    case SDL_SCANCODE_UNKNOWN:
                        continue;

                    case static_cast<SDL_Scancode>((int)SDL_SCANCODE_COUNT +
                                                   1):  // left click
                    case static_cast<SDL_Scancode>((int)SDL_SCANCODE_COUNT +
                                                   2):  // middle click
                    case static_cast<SDL_Scancode>((int)SDL_SCANCODE_COUNT +
                                                   3):  // right click
                    case static_cast<SDL_Scancode>((int)SDL_SCANCODE_COUNT +
                                                   4):  // side1 click
                    case static_cast<SDL_Scancode>((int)SDL_SCANCODE_COUNT +
                                                   5):  // side2 click
                        if (SDL_GetMouseState(NULL, NULL) |
                            SDL_BUTTON_MASK(1 + Scancode -
                                            SDL_SCANCODE_COUNT)) {
                            goto ReturnTrue;
                        }
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

        static int Released(lua_State* State) {
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

        static int GetScancodes(lua_State* State) {
            int TableIndex = 0;

            const std::vector<SDL_Scancode>& SDLScancodes =
                static_cast<Binding*>(
                    LuaHelper::CheckMetatable(State, 1, lua_upvalueindex(1)))
                    ->SDLScancodes;

            lua_createtable(State, 0, SDLScancodes.size());
            for (const auto Scancode : SDLScancodes) {
                if (Scancode != SDL_SCANCODE_UNKNOWN) {
                    // sub by one bc function doesn't account for pseudo indexes
                    LuaHelper::SetIndex<lua_Integer>(
                        State, -2 - 1, static_cast<lua_Integer>(Scancode),
                        ++TableIndex);
                }
            }

            return 1;
        }

        static int AddScancode(lua_State* State) {
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
                    for (; ScancodeIndex < SDLScancodes.size();
                         ++ScancodeIndex) {
                        unlikely_branch if (SDLScancodes[ScancodeIndex] ==
                                            Scancode) {
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

        static int RemoveScancode(lua_State* State) {
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

        static int ClearScancodes(lua_State* State) {
            static_cast<Binding*>(
                luaL_checkudata(State, 1, Binding::MetatableName))
                ->SDLScancodes.clear();
            return 0;
        }

        [[noreturn]]
        static int __newindex(lua_State* State) {
            luaL_error(State,
                       "Attempted to write to a Binding object. Binding is "
                       "read-only.");
        }
    };

    static int GetBinding(lua_State* State);
#endif
}

void Game::Lua::CLibraries::Input::Init(lua_State* State) {
    LuaHelper::StackTableReference EnumScancode, BindingsTable,
        BindingMetatable;
    // const char* KeyName;

    lua_getglobal(State, "Enums");
    lua_pushliteral(State, "Scancode");

    EnumScancode = LuaHelper::StackTableReference(
        State, 0, static_cast<int>(SDL_Scancode::SDL_SCANCODE_COUNT));

    // cursed fuckery to make a constant static buffer of "A\0B\0..."
    /*constexpr const static struct _AllCharactersStruct {
            constexpr _AllCharactersStruct() {

                    char Letter = 'A', i = 0;

                    for (; Letter <= 'Z'; ++Letter) {
                            this->Characters[i] = Letter;
                            this->Characters[i + 1] = '\0';
                            i += 2;
                    }
            }
            char Characters[2 + ('Z' - 'A') * 2]{};

    } _AllCharacters;

    int LetterIndex = 0;
    for (int i = 0; i < 'Z' - 'A'; ++i) {

            lua_pushlstring(State, &_AllCharacters.Characters[LetterIndex], 1);
            //lua_pushexternalstring(State,
    &_AllCharacters.Characters[LetterIndex], 1, NULL, NULL);
            //lua_pushlstring(State, &_AllCharacters.Characters[LetterIndex],
    2); CLibraries::Enums::PushNewEnum(State, i + SDL_SCANCODE_A);
            lua_rawset(State, EnumScancode.GetStackIndex());
            //lua_setfield(State, EnumScancode.GetStackIndex(),
    &_AllCharacters.Characters[LetterIndex]);

            LetterIndex += 2;
    }*/

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

#ifndef BUILD_SERVER
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

#ifndef BUILD_SERVER

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