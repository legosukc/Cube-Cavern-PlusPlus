#pragma once

#include <SDL3_mixer/SDL_mixer.h>

#include "../../MathClasses/Vector3.hpp"

#include "../SoundClasses/Listener.hpp"


namespace Game::Sound::SDLMixer {

	extern MIX_Track* DefaultTrack;
	extern MIX_Mixer* Mixer;

	struct Listener_SDLMixer : Game::Sound::Classes::ListenerStruct {
		virtual void SetPosition(const Math::Vector3& Position) override;
	};

	inline bool Init();
	inline void Destroy();
}