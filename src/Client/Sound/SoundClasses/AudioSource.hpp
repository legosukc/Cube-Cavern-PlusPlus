#pragma once

#include <AL/al.h>
#include <AL/alext.h>

namespace Game::Sound::Classes {

    class AudioSource {
        static AudioSource* Create();
        static void CreateBulk(size_t Count, AudioSource* AudioSources[]);

       public:
        virtual ~AudioSource();

        virtual void UploadSoundData(const void* SoundData,
                                     size_t SoundSize,
                                     ALenum SoundFormat,
                                     ALsizei SampleRate = 441000);
        virtual bool LoadFile(const char* Path,
                              ALenum Format = AL_FORMAT_STEREO_FLOAT32);
    };
}

namespace Game::Sound {
    extern Game::Sound::Classes::AudioSource* (*CreateAudioSource)();
    extern void (*CreateAudioSourceBulk)(size_t Count,
                                  Game::Sound::Classes::AudioSource* Sources[]);
}