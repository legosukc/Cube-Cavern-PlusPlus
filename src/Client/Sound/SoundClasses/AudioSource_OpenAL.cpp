#include "AudioSource_OpenAL.hpp"

#include <cstdlib>

#include <AL/al.h>

#include <SDL3/SDL_stdinc.h>

#include "../SoundUtils.hpp"
#include "AudioSource.hpp"

namespace Game::Sound::OpenAL::AudioSource {
    Game::Sound::Classes::AudioSource* Create();
    void CreateBulk(size_t Count,
                    Game::Sound::Classes::AudioSource* AudioSources[]);
}

namespace Game::Sound::Classes {

    Game::Sound::Classes::AudioSource* AudioSource_OpenAL::Create() {
        AudioSource_OpenAL* NewSpeaker = new AudioSource_OpenAL();
        alGenBuffers(1, &NewSpeaker->ALBuffer);
        return reinterpret_cast<Game::Sound::Classes::AudioSource*>(NewSpeaker);
    }

    void AudioSource_OpenAL::CreateBulk(
        size_t Count,
        Game::Sound::Classes::AudioSource* AudioSources[]) {
        ALuint* ALBuffers = SDL_stack_alloc(ALuint, Count);
        alGenBuffers(Count, ALBuffers);

        for (size_t i = 0; i < Count; ++i) {
            AudioSources[i] = new AudioSource_OpenAL;
            reinterpret_cast<AudioSource_OpenAL*>(AudioSources[i])->ALBuffer =
                ALBuffers[i];
        }
        SDL_stack_free(ALBuffers);
    }

    AudioSource_OpenAL::~AudioSource_OpenAL() {
        alDeleteBuffers(1, &this->ALBuffer);
    }

    void AudioSource_OpenAL::UploadSoundData(
        const void* SoundData,
        size_t SoundSize,
        ALenum SoundFormat,
        ALsizei SampleRate) {
        alBufferData(this->ALBuffer, SoundFormat, SoundData, SoundSize,
                     SampleRate);
    }

    bool AudioSource_OpenAL::LoadFile(
        const char* Path,
        ALenum Format) {
        size_t AudioSize;
        Game::Sound::Utils::RawAudioBuffer AudioData =
            Game::Sound::Utils::DecodeAudioFile(Path, &AudioSize);

        alBufferData(this->ALBuffer, Format, AudioData.AudioChannelData,
                     AudioSize, 441000);

        return true;
    }
}

Game::Sound::Classes::AudioSource* Game::Sound::OpenAL::AudioSource::Create() {
    Game::Sound::Classes::AudioSource_OpenAL* NewSpeaker =
        new Game::Sound::Classes::AudioSource_OpenAL();
    alGenBuffers(1, &NewSpeaker->ALBuffer);
    return reinterpret_cast<Game::Sound::Classes::AudioSource*>(NewSpeaker);
}

void Game::Sound::OpenAL::AudioSource::CreateBulk(
    size_t Count,
    Game::Sound::Classes::AudioSource* AudioSources[]) {
    ALuint* ALBuffers = SDL_stack_alloc(ALuint, Count);
    alGenBuffers(Count, ALBuffers);

    for (size_t i = 0; i < Count; ++i) {
        AudioSources[i] = new Game::Sound::Classes::AudioSource_OpenAL;
        reinterpret_cast<Game::Sound::Classes::AudioSource_OpenAL*>(
            AudioSources[i])
            ->ALBuffer = ALBuffers[i];
    }
    SDL_stack_free(ALBuffers);
}