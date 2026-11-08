#include "LuaSurface.hpp"

#include "../../include/VM/lua.h"
#include "../../include/VM/lualib.h"

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>

int Game::Lua::CLibraries::Surface::BufferLoadImage(lua_State* State) {
    SDL_Surface* Surface = IMG_Load(luaL_checkstring(State, 1));

    lua_createtable(State, 0, 2);

    const size_t SurfaceSize = Surface->h * Surface->pitch;
    memcpy(lua_newbuffer(State, SurfaceSize), Surface->pixels, SurfaceSize);

    lua_setfield(State, -2, "Buffer");

    lua_pushinteger(State, Surface->format);
    lua_setfield(State, -2, "Format");

    SDL_DestroySurface(Surface);

    return 1;
}

int Game::Lua::CLibraries::Surface::ConvertBufferTo(lua_State* State) {
    SDL_Surface* Surface = IMG_Load(luaL_checkstring(State, 1));

    const size_t SurfaceSize = Surface->h * Surface->pitch;
    memcpy(lua_newbuffer(State, SurfaceSize), Surface->pixels, SurfaceSize);

    SDL_DestroySurface(Surface);

    return 1;
}