#pragma once

#include <cstdlib>

#include <AL/al.h>

#include <SDL3/SDL_stdinc.h>

#include "AudioSource.hpp"

namespace Game::Sound::OpenAL::AudioSource {
    Game::Sound::Classes::AudioSource* Create();
    void CreateBulk(size_t Count,
                    Game::Sound::Classes::AudioSource* AudioSources[]);
}

namespace Game::Sound::Classes {

    class AudioSource_OpenAL : Game::Sound::Classes::AudioSource {
        ALuint ALBuffer;

        static Game::Sound::Classes::AudioSource* Create();

        static void CreateBulk(
            size_t Count,
            Game::Sound::Classes::AudioSource* AudioSources[]) ;

        friend Game::Sound::Classes::AudioSource*
            Game::Sound::OpenAL::AudioSource::Create();
        friend void Game::Sound::OpenAL::AudioSource::CreateBulk(
            size_t Count,
            Game::Sound::Classes::AudioSource* AudioSources[]);

       public:
        virtual ~AudioSource_OpenAL() override;

        virtual void UploadSoundData(const void* SoundData,
                                     size_t SoundSize,
                                     ALenum SoundFormat,
                                     ALsizei SampleRate = 441000) override;

        virtual bool LoadFile(
            const char* Path,
            ALenum Format = AL_FORMAT_STEREO_FLOAT32) override;
    };
}