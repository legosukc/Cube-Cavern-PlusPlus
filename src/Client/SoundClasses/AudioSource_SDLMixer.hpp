#pragma once

#ifndef AUDIOSOURCE_SDLMIXER_H
#define AUDIOSOURCE_SDLMIXER_H

#include <AL/al.h>
#include <AL/alext.h>

#include <SDL3_mixer/SDL_mixer.h>

#include "AudioSource.hpp"

#include "../SoundAPIHandlers/SDLMixer_Sound.hpp"

namespace Game::Sound::Classes {

    class AudioSource_SDLMixer : Game::Sound::Classes::AudioSource {
        MIX_Audio* Audio;

        friend bool Game::Sound::SDLMixer::Init();

       public:
        virtual ~AudioSource_SDLMixer() override {
            MIX_DestroyAudio(this->Audio);
        }

        // TODO: implement
        virtual void UploadSoundData(const void* SoundData,
                                     size_t SoundSize,
                                     ALenum SoundFormat,
                                     ALsizei SampleRate = 441000) override {}

        // TODO: implement
        virtual bool LoadFile(
            const char* Path,
            ALenum Format = AL_FORMAT_STEREO_FLOAT32) override {
            return false;
        }
    };

}

#endif