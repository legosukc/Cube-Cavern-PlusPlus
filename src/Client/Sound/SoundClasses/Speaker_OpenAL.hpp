#ifndef SOUND_SPEAKER_OPENAL_H
#define SOUND_SPEAKER_OPENAL_H

#include <AL/al.h>

#include "../../../MathClasses/Vector3.hpp"

#include "Speaker.hpp"

namespace Game::Sound::OpenAL::Speaker {
    Game::Sound::Classes::Speaker* Create();

    void CreateBulk(size_t Count, Game::Sound::Classes::Speaker* Speakers[]);
}

namespace Game::Sound::Classes {

    class Speaker_OpenAL : public Game::Sound::Classes::Speaker {
        ALuint ALSource = 0;
        Game::Sound::Classes::AudioSource* BoundAudioSource = NULL;

        friend Game::Sound::Classes::Speaker*
            Game::Sound::OpenAL::Speaker::Create();
        friend void Game::Sound::OpenAL::Speaker::CreateBulk(
            size_t Count,
            Game::Sound::Classes::Speaker* Speakers[]);

       public:
        virtual ~Speaker_OpenAL() override {
            alDeleteSources(1, &this->ALSource);
        }

        virtual void SetAudioSource(
            Game::Sound::Classes::AudioSource* AudioSource) override {
            alSourcei(this->ALSource, AL_BUFFER,
                      *reinterpret_cast<const ALuint*>(AudioSource));
            this->BoundAudioSource = AudioSource;
        }
        virtual Game::Sound::Classes::AudioSource* GetAudioSource()
            const override {
            return this->BoundAudioSource;
        }

        virtual void Play() override { alSourcePlay(this->ALSource); }

        virtual void Pause() override { alSourcePause(this->ALSource); }

        virtual void Stop() override { alSourceStop(this->ALSource); }

        virtual void SetPosition(const Math::Vector3& Position) override {
            alSourcefv(this->ALSource, AL_POSITION,
                       reinterpret_cast<const ALfloat*>(&Position));
        }

        virtual void SetVelocity(const Math::Vector3& Velocity) override {
            alSourcefv(this->ALSource, AL_VELOCITY,
                       reinterpret_cast<const ALfloat*>(&Velocity));
        }

        virtual void SetOrientation(const Math::Vector3& Look,
                                    const Math::Vector3& Up) override {
            const Math::Vector3 Orientation[2] = {Look, Up};
            alSourcefv(this->ALSource, AL_ORIENTATION,
                       reinterpret_cast<const ALfloat*>(&Orientation));
        }
    };
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

#endif