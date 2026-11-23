#include "Speaker_OpenAL.hpp"

#include <AL/al.h>

#include "../../../MathClasses/Vector3.hpp"

#include "Speaker.hpp"

namespace Game::Sound::Classes {

    Speaker_OpenAL::~Speaker_OpenAL() {
        alDeleteSources(1, &this->ALSource);
    }

    void Speaker_OpenAL::SetAudioSource(
        Game::Sound::Classes::AudioSource* AudioSource) {
        alSourcei(this->ALSource, AL_BUFFER,
                  *reinterpret_cast<const ALuint*>(AudioSource));
        this->BoundAudioSource = AudioSource;
    }

    Game::Sound::Classes::AudioSource* Speaker_OpenAL::GetAudioSource() const {
        return this->BoundAudioSource;
    }

    void Speaker_OpenAL::Play() {
        alSourcePlay(this->ALSource);
    }

    void Speaker_OpenAL::Pause() {
        alSourcePause(this->ALSource);
    }

    void Speaker_OpenAL::Stop() {
        alSourceStop(this->ALSource);
    }

    void Speaker_OpenAL::SetPosition(const Math::Vector3& Position) {
        alSourcefv(this->ALSource, AL_POSITION,
                   reinterpret_cast<const ALfloat*>(&Position));
    }

    void Speaker_OpenAL::SetVelocity(const Math::Vector3& Velocity) {
        alSourcefv(this->ALSource, AL_VELOCITY,
                   reinterpret_cast<const ALfloat*>(&Velocity));
    }

    void Speaker_OpenAL::SetOrientation(const Math::Vector3& Look,
                                        const Math::Vector3& Up) {
        const Math::Vector3 Orientation[2] = {Look, Up};
        alSourcefv(this->ALSource, AL_ORIENTATION,
                   reinterpret_cast<const ALfloat*>(&Orientation));
    }
}

Game::Sound::Classes::Speaker* Game::Sound::OpenAL::Speaker::Create() {
    Game::Sound::Classes::Speaker_OpenAL* NewSpeaker =
        new Game::Sound::Classes::Speaker_OpenAL();
    alGenSources(1, &NewSpeaker->ALSource);
    return reinterpret_cast<Game::Sound::Classes::Speaker*>(NewSpeaker);
}

void Game::Sound::OpenAL::Speaker::CreateBulk(
    size_t Count,
    Game::Sound::Classes::Speaker* Speakers[]) {
    ALuint* ALSources = SDL_stack_alloc(ALuint, Count);
    alGenSources(Count, ALSources);

    for (size_t i = 0; i < Count; ++i) {
        Speakers[i] = new Game::Sound::Classes::Speaker_OpenAL;
        reinterpret_cast<Game::Sound::Classes::Speaker_OpenAL*>(Speakers[i])
            ->ALSource = ALSources[i];
    }
    SDL_stack_free(ALSources);
}