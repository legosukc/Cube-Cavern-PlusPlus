#pragma once

#include "../../../include/VM/lua.h"

#include "../../FunctionHeaders/LuaHelper.hpp"

namespace Game::Lua::CLibraries::Sound {
    inline void Init(lua_State* State,
                     LuaHelper::StackTableReference& GameTable);

    int SetListenerPosition(lua_State* State);

    int SetListenerVelocity(lua_State* State);

    int SetListenerOrientation(lua_State* State);

    int CreateAudioSource(lua_State* State);
    int CreateSpeaker(lua_State* State);

    struct AudioSourceMetatable {};

    struct SpeakerMetatable {
        static int Play(lua_State* State);
        static int Stop(lua_State* State);
        static int Pause(lua_State* State);
    };
}

void Game::Lua::CLibraries::Sound::Init(
    lua_State* State,
    LuaHelper::StackTableReference& GameTable) {
    LuaHelper::StackTableReference AudioSourceMetatable(State, "AudioSource");

    AudioSourceMetatable.SetKeyClosure(
        State, CLibraries::Sound::SetListenerPosition, "SetListenerPosition");

    LuaHelper::StackTableReference SpeakerMetatable(State, "Speaker");

    SpeakerMetatable.SetKeyClosure(
        State, CLibraries::Sound::SpeakerMetatable::Play, "Play");
    SpeakerMetatable.SetKeyClosure(
        State, CLibraries::Sound::SpeakerMetatable::Pause, "Pause");
    SpeakerMetatable.SetKeyClosure(
        State, CLibraries::Sound::SpeakerMetatable::Stop, "Stop");

    lua_settop(State, AudioSourceMetatable.GetStackIndex() - 1);

    lua_createtable(State, 0, 5);

    LuaHelper::SetKeyClosure(State, -2, CLibraries::Sound::SetListenerPosition,
                             "SetListenerPosition");
    LuaHelper::SetKeyClosure(State, -2, CLibraries::Sound::SetListenerVelocity,
                             "SetListenerVelocity");
    LuaHelper::SetKeyClosure(State, -2,
                             CLibraries::Sound::SetListenerOrientation,
                             "SetListenerOrientation");

    LuaHelper::SetKeyClosure(State, -2, CLibraries::Sound::CreateAudioSource,
                             "CreateAudioSource");
    LuaHelper::SetKeyClosure(State, -2, CLibraries::Sound::CreateSpeaker,
                             "CreateSpeaker");

    lua_setfield(State, GameTable.GetStackIndex(), "Sound");
}