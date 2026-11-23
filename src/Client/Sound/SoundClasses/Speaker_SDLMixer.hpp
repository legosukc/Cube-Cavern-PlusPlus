#pragma once

#include <SDL3/SDL_stdinc.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "../../../MathClasses/Vector3.hpp"

#include "Speaker.hpp"

#include "../SoundAPIHandlers/SDLMixer_Sound.hpp"

namespace Game::Sound::Classes {

	class Speaker_SDLMixer : public Game::Sound::Classes::Speaker {
		MIX_Audio* AudioBuffer;

		friend bool Game::Sound::SDLMixer::Init();

	public:
		virtual ~Speaker_SDLMixer() override;

		virtual void Play() override;
		virtual void Pause() override;
		virtual void Stop() override;

		virtual void SetPosition(const Math::Vector3& Position) override;
	};
}