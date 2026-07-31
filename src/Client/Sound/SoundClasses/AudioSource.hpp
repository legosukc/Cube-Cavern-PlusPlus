#ifndef SOUND_AUDIOSORUCE_H
#define SOUND_AUDIOSORUCE_H

#include <iostream>

#include <AL/al.h>
#include <AL/alext.h>

namespace Game::Sound::Classes {

    class AudioSource {
        static AudioSource* Create();
        static void CreateBulk(size_t Count, AudioSource* AudioSources[]);

       public:
        virtual ~AudioSource() = default;

        virtual void UploadSoundData(const void* SoundData,
                                     size_t SoundSize,
                                     ALenum SoundFormat,
                                     ALsizei SampleRate = 441000) {}
        virtual bool LoadFile(const char* Path,
                              ALenum Format = AL_FORMAT_STEREO_FLOAT32) {
            std::cerr
                << "Called uninitialized or unoverriden virtual "
                   "function 'Game::Sound::Classes::AudioSource::LoadFile()'."
                << std::endl;
#ifdef DEBUG_BUILD

#endif
            return false;
        }
    };
}

namespace Game::Sound {
    Game::Sound::Classes::AudioSource* (*CreateAudioSource)();
    void (*CreateAudioSourceBulk)(size_t Count,
                                  Game::Sound::Classes::AudioSource* Sources[]);
}

#endif