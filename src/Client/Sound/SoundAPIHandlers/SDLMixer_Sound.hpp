#ifndef SOUND_SDLMIXER_H
#define SOUND_SDLMIXER_H

#include "../../../define.h"

#include <iostream>

#include <SDL3_mixer/SDL_mixer.h>

#include "../../../MathClasses/Vector3.hpp"

#include "../SoundClasses/Listener.hpp"

namespace Game::Sound::SDLMixer {

    MIX_Track* DefaultTrack;
    MIX_Mixer* Mixer;

    struct Listener_SDLMixer : Game::Sound::Classes::ListenerStruct {
        virtual void SetPosition(const Math::Vector3& Position) override;
    };

    inline bool Init();
    inline void Destroy();
}

void Game::Sound::SDLMixer::Listener_SDLMixer::SetPosition(
    const Math::Vector3& Position) {
    MIX_Point3D CastPosition = {Position.X, Position.Y, Position.Z};
    MIX_SetTrack3DPosition(Game::Sound::SDLMixer::DefaultTrack, &CastPosition);
}

bool Game::Sound::SDLMixer::Init() {
    // open default device
    Game::Sound::SDLMixer::Mixer =
        MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (Game::Sound::SDLMixer::Mixer == NULL) {
        std::clog << "Failed to create SDL_Mixer mixer";
        return false;
    }

    Game::Sound::Listener = Game::Sound::SDLMixer::Listener_SDLMixer();

    return true;
}

void Game::Sound::SDLMixer::Destroy() {
    Game::Sound::Listener = Game::Sound::Classes::ListenerStruct();

    if (Game::Sound::SDLMixer::Mixer != NULL) {
        MIX_DestroyMixer(Game::Sound::SDLMixer::Mixer);
        Game::Sound::SDLMixer::Mixer = NULL;
    }
}
#endif