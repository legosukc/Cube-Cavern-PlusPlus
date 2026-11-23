#pragma once

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
        virtual ~Speaker_OpenAL() override;

        virtual void SetAudioSource(
            Game::Sound::Classes::AudioSource* AudioSource) override;

        virtual Game::Sound::Classes::AudioSource* GetAudioSource()
            const override;

        virtual void Play() override;

        virtual void Pause() override;

        virtual void Stop() override;

        virtual void SetPosition(const Math::Vector3& Position) override;

        virtual void SetVelocity(const Math::Vector3& Velocity) override;

        virtual void SetOrientation(const Math::Vector3& Look,
                                    const Math::Vector3& Up) override;
    };
}