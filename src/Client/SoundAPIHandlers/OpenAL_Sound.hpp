#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include "../../MathClasses/Vector3.hpp"

#include "../SoundClasses/Listener.hpp"

namespace Game::Sound::OpenAL {

	extern ALCdevice* Device;
	extern ALCcontext* Context;

	struct Listener_OpenAL : Game::Sound::Classes::ListenerStruct {
		virtual void SetPosition(const Math::Vector3& Position) override;
		virtual void SetVelocity(const Math::Vector3& Velocity) override;
		virtual void SetOrientation(const Math::Vector3& Look, const Math::Vector3& Up) override;
	};

	inline bool Init();
	inline void Destroy();
}