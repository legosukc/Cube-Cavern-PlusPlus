#pragma once

#include <cstddef>
#include <cstdlib>
#include <cstring>

#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>

namespace Game::Sound::Utils {

    class RawAudioBuffer {
        void(*AudioChannelDataDeallocater)(void*) = NULL;
    public:
        inline ~RawAudioBuffer() {
            this->AudioChannelDataDeallocater(this->AudioChannelData);
        }
        void** AudioChannelData;
        Uint32 AudioSize;
        Uint32 SampleRate;
        Uint8 Channels;

        friend RawAudioBuffer DecodeAudioFile(const char* Path,
                                              size_t* AudioSize);
    };

    // Free returned PCM data using "std::free".
    RawAudioBuffer DecodeAudioFile(const char* Path, size_t* AudioSize);
}