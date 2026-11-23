#include "AudioSource_SDLMixer.hpp"

#include <AL/al.h>
#include <AL/alext.h>

#include <SDL3_mixer/SDL_mixer.h>

namespace Game::Sound::Classes {

    AudioSource_SDLMixer::~AudioSource_SDLMixer() {
        MIX_DestroyAudio(this->Audio);
    }

    // TODO: implement
    void AudioSource_SDLMixer::UploadSoundData(const void* SoundData,
                                               size_t SoundSize,
                                               ALenum SoundFormat,
                                               ALsizei SampleRate) {}

    // TODO: implement
    bool AudioSource_SDLMixer::LoadFile(const char* Path, ALenum Format) {
        return false;
    }
}