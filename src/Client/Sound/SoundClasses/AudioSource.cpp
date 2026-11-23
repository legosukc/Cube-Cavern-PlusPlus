#include "AudioSource.hpp"

#include <iostream>

#include <AL/al.h>
#include <AL/alext.h>

namespace Game::Sound::Classes {

    AudioSource::~AudioSource() = default;

    void AudioSource::UploadSoundData(const void* SoundData,
                                              size_t SoundSize,
                                              ALenum SoundFormat,
                                              ALsizei SampleRate) {}
    bool AudioSource::LoadFile(
        const char* Path,
        ALenum Format) {
        std::cerr << "Called uninitialized or unoverriden virtual "
                     "function 'Game::Sound::Classes::AudioSource::LoadFile()'."
                  << std::endl;
#ifdef DEBUG_BUILD

#endif
        return false;
    }
}

namespace Game::Sound {
    Game::Sound::Classes::AudioSource* (*CreateAudioSource)() = NULL;
    
    void (*CreateAudioSourceBulk)(
        size_t Count,
        Game::Sound::Classes::AudioSource* Sources[]) = NULL;
}