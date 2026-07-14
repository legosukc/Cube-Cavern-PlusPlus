#pragma once

#include "AudioSource.hpp"

#include "../../MathClasses/Vector3.hpp"


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