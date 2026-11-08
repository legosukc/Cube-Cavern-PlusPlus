#pragma once

#include "../../include/VM/lua.h"

#include <SDL3/SDL_stdinc.h>

namespace Game::Lua::CLibraries::Surface {

    inline void Init(lua_State* State);

    namespace {
        template <typename ColorType,
                  Uint8 RBits = 0,
                  Uint8 GBits = 0,
                  Uint8 BBits = 0,
                  Uint8 ABits = 0>
        struct _FormatBase {
            constexpr static Uint8 RedBits = RBits;
            constexpr static Uint8 GreenBits = GBits;
            constexpr static Uint8 BlueBits = BBits;
            constexpr static Uint8 AlphaBits = ABits;

            using Type = ColorType;
        };
    }

    using R8I = _FormatBase<Sint8, 8>;
    using R16I = _FormatBase<Sint16, 16>;
    using R32I = _FormatBase<Sint32, 32>;

    using R8UI = _FormatBase<Uint8, 8>;
    using R16UI = _FormatBase<Uint16, 16>;
    using R32UI = _FormatBase<Uint32, 32>;

    using R32F = _FormatBase<float, 32>;

    using RG8I = _FormatBase<Sint8, 8, 8>;
    using RG16I = _FormatBase<Sint16, 16, 16>;
    using RG32I = _FormatBase<Sint32, 32, 32>;

    using RG8UI = _FormatBase<Uint8, 8, 8>;
    using RG16UI = _FormatBase<Uint16, 16, 16>;
    using RG32UI = _FormatBase<Uint32, 32, 32>;

    using RG32F = _FormatBase<float, 32, 32>;

    using RGB8I = _FormatBase<Sint8, 8, 8, 8>;
    using RGB16I = _FormatBase<Sint16, 16, 16, 16>;
    using RGB32I = _FormatBase<Sint32, 32, 32, 32>;

    using RGB8UI = _FormatBase<Uint8, 8, 8, 8>;
    using RGB16UI = _FormatBase<Uint16, 16, 16, 16>;
    using RGB32UI = _FormatBase<Uint32, 32, 32, 32>;

    using RGB32F = _FormatBase<float, 32, 32, 32>;

    using RGBA8I = _FormatBase<Sint8, 8, 8, 8, 8>;
    using RGBA16I = _FormatBase<Sint16, 16, 16, 16, 16>;
    using RGBA32I = _FormatBase<Sint32, 32, 32, 32, 32>;

    using RGBA8UI = _FormatBase<Uint8, 8, 8, 8, 8>;
    using RGBA16UI = _FormatBase<Uint16, 16, 16, 16, 16>;
    using RGBA32UI = _FormatBase<Uint32, 32, 32, 32, 32>;

    using RGBA32F = _FormatBase<float, 32, 32, 32, 32>;

    int BufferLoadImage(lua_State* State);
    int ConvertBufferTo(lua_State* State);

    constexpr char LibraryName[] = "Surface";
}

void Game::Lua::CLibraries::Surface::Init(lua_State* State) {
    lua_createtable(State, 0, 2);

    lua_pushcfunction(State, Surface::BufferLoadImage,
                      "Surface.BufferLoadImage");
    lua_setfield(State, -2, "BufferLoadImage");

    lua_pushcfunction(State, Surface::ConvertBufferTo,
                      "Surface.ConvertBufferTo");
    lua_setfield(State, -2, "ConvertBufferTo");

    lua_setglobal(State, CLibraries::Surface::LibraryName);
}