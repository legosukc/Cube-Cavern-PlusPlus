#ifndef SOUND_CLIENT_H
#define SOUND_CLIENT_H

#include "../../define.h"

#include <iostream>

#include <AL/al.h>
#include <AL/alext.h>

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>

//#ifndef SDL_PLATFORM_VITA
//#include <SDL3_mixer/SDL_mixer.h>
//#endif

#include "../../FunctionHeaders/Exceptions.hpp"

#include "SoundUtils.hpp"

#include "SoundClasses/AudioSource.hpp"
#include "SoundClasses/Listener.hpp"
#include "SoundClasses/Speaker.hpp"

#include "SoundAPIHandlers/OpenAL_Sound.hpp"

#ifndef SDL_PLATFORM_VITA
#include "SoundAPIHandlers/SDLMixer_Sound.hpp"
#endif

namespace Game::Sound {

    enum class AudioBackendEnum : Uint8 {
        OpenAL,
        SDL_Mixer,
        Cubeb,
        NoAudio,
    };
    AudioBackendEnum AudioBackend = AudioBackendEnum::NoAudio;

    bool AudioEnabled = false;

    inline bool Init();
    inline void Destroy();

    using Classes::AudioSource;
    using Classes::Speaker;
}

bool Game::Sound::Init() {
    std::cout << "Initalizing Sound." << std::endl;
    const Uint64 StartMS = SDL_GetTicks();

    #ifndef SDL_PLATFORM_VITA
    if (!MIX_Init()) {
        Exceptions::ThrowSDLError(
            "Failed to initialize SDL_mixer (needed for decoding audio, even "
            "if you selected another SoundAPI.)");
    }
    #endif

    Game::Sound::AudioBackend = Game::Sound::AudioBackendEnum::OpenAL;

    bool Success;
    switch (Game::Sound::AudioBackend) {
        case AudioBackendEnum::OpenAL:
            Success = Game::Sound::OpenAL::Init();
            break;
        #ifndef SDL_PLATFORM_VITA
        case AudioBackendEnum::SDL_Mixer:
            Success = Game::Sound::SDLMixer::Init();
            break;
        #endif
    }

    Game::Sound::AudioEnabled = Success;
    if (Success) {
        std::cout << "Initialized Sound";
    } else {
        Game::Sound::Destroy();
        std::clog << ".\nFailed to initalize Sound, audio has been disabled";
    }
    std::clog << ", took " << SDL_GetTicks() - StartMS << " milliseconds."
              << std::endl;

    return Success;
}

void Game::Sound::Destroy() {
    switch (Game::Sound::AudioBackend) {
        case AudioBackendEnum::OpenAL:
            Game::Sound::OpenAL::Destroy();
            break;
        #ifndef SDL_PLATFORM_VITA
        case AudioBackendEnum::SDL_Mixer:
            Game::Sound::SDLMixer::Destroy();
            break;
        #endif
    }
}

#endif