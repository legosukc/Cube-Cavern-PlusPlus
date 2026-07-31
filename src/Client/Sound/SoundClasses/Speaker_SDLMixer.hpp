#pragma once

#include <iostream>

#include <SDL3/SDL_stdinc.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "../../MathClasses/Vector3.hpp"

#include "Speaker.hpp"


namespace Game::Sound::Classes {

	class Speaker_SDLMixer : public Game::Sound::Classes::Speaker {
		MIX_Audio* AudioBuffer;

		friend bool Game::Sound::SDLMixer::Init();

	public:
		virtual ~Speaker_SDLMixer() override {
			MIX_DestroyAudio(this->AudioBuffer);
		}

		virtual void Play() override {
			MIX_PlayAudio(Game::Sound::SDLMixer::Mixer, this->AudioBuffer);
		}

		virtual void Pause() override {
			MIX_PauseTrack(Game::Sound::SDLMixer::DefaultTrack);
		}

		virtual void Stop() override {
			MIX_StopTrack(Game::Sound::SDLMixer::DefaultTrack, 0);
		}


		virtual void SetPosition(const Math::Vector3& Position) override {

		}
	};
}