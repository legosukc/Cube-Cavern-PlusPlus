#pragma once

#include "AudioSource.hpp"

#include "../../../MathClasses/Vector3.hpp"


namespace Game::Sound::Classes {
	class Speaker {
	public:
		virtual ~Speaker();

		virtual void SetAudioSource(Game::Sound::Classes::AudioSource* AudioSource);
		virtual Game::Sound::Classes::AudioSource* GetAudioSource() const;

		virtual void Play();
		virtual void Pause();
		virtual void Stop();

		virtual void SetPosition(const Math::Vector3& Position);
		virtual void SetVelocity(const Math::Vector3& Velocity);
		virtual void SetOrientation(const Math::Vector3& Look, const Math::Vector3& Up);
	};
}

namespace Game::Sound {
    extern Game::Sound::Classes::Speaker* (*CreateSpeaker)();
    extern void (*CreateSpeakerBulk)(size_t Count,
                                     Game::Sound::Classes::Speaker* Speakers[]);
}