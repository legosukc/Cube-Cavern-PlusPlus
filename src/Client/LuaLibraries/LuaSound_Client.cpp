#include "LuaSound_Client.hpp"

#include "../../../include/VM/lua.h"
#include "../../../include/VM/lualib.h"

#include "../Sound/Sound.hpp"

#include "../../MathClasses/Vector3.hpp"


int Game::Lua::CLibraries::Sound::SetListenerPosition(lua_State* State) {
    Game::Sound::Listener.SetPosition(
        *static_cast<Math::Vector3*>(luaL_checkudata(State, 1, "Vector3")));
    return 0;
}

int Game::Lua::CLibraries::Sound::SetListenerVelocity(lua_State* State) {
    Game::Sound::Listener.SetVelocity(
        *static_cast<Math::Vector3*>(luaL_checkudata(State, 1, "Vector3")));
    return 0;
}

int Game::Lua::CLibraries::Sound::SetListenerOrientation(lua_State* State) {
    Game::Sound::Listener.SetOrientation(
        *static_cast<Math::Vector3*>(luaL_checkudata(State, 1, "Vector3")),
        *static_cast<Math::Vector3*>(luaL_checkudata(State, 2, "Vector3")));
    return 0;
}

int Game::Lua::CLibraries::Sound::CreateAudioSource(lua_State* State) {
    Game::Sound::Classes::AudioSource** AudioSourceUD =
        static_cast<Game::Sound::Classes::AudioSource**>(
            lua_newuserdata(State, sizeof(Game::Sound::Classes::AudioSource*)));
    *AudioSourceUD = Game::Sound::CreateAudioSource();

    luaL_getmetatable(State, "AudioSource");
    lua_setmetatable(State, -2);

    return 1;
}

int Game::Lua::CLibraries::Sound::CreateSpeaker(lua_State* State) {
    Game::Sound::Classes::Speaker** SpeakerUD =
        static_cast<Game::Sound::Classes::Speaker**>(
            lua_newuserdata(State, sizeof(Game::Sound::Classes::Speaker*)));
    *SpeakerUD = Game::Sound::CreateSpeaker();

    luaL_getmetatable(State, "Speaker");
    lua_setmetatable(State, -2);

    return 1;
}

int Game::Lua::CLibraries::Sound::SpeakerMetatable::Play(lua_State* State) {
    (*static_cast<Game::Sound::Classes::Speaker**>(
         luaL_checkudata(State, 1, "Speaker")))
        ->Play();
    return 0;
}

int Game::Lua::CLibraries::Sound::SpeakerMetatable::Stop(lua_State* State) {
    (*static_cast<Game::Sound::Classes::Speaker**>(
         luaL_checkudata(State, 1, "Speaker")))
        ->Stop();
    return 0;
}

int Game::Lua::CLibraries::Sound::SpeakerMetatable::Pause(lua_State* State) {
    (*static_cast<Game::Sound::Classes::Speaker**>(
         luaL_checkudata(State, 1, "Speaker")))
        ->Pause();
    return 0;
}