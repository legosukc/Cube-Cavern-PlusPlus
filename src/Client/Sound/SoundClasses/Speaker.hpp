#ifndef SOUND_SPEAKER_H
#define SOUND_SPEAKER_H

#include "AudioSource.hpp"

#include "../../../MathClasses/Vector3.hpp"


namespace Game::Sound::Classes {
	class Speaker {
	public:
		virtual ~Speaker() = default;

		virtual void SetAudioSource(Game::Sound::Classes::AudioSource* AudioSource) {}
		virtual Game::Sound::Classes::AudioSource* GetAudioSource() const {
			return new Game::Sound::Classes::AudioSource();
		}

		virtual void Play() {}
		virtual void Pause() {}
		virtual void Stop() {}

		virtual void SetPosition(const Math::Vector3& Position) {}
		virtual void SetVelocity(const Math::Vector3& Velocity) {}
		virtual void SetOrientation(const Math::Vector3& Look, const Math::Vector3& Up) {}
	};
}

namespace Game::Sound {
    Game::Sound::Classes::Speaker* (*CreateSpeaker)();
    void (*CreateSpeakerBulk)(size_t Count,
                              Game::Sound::Classes::Speaker* Speakers[]);
}

#endif