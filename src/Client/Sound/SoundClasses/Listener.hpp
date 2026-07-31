#ifndef SOUND_LISTENER_H
#define SOUND_LISTENER_H

#include "../../../MathClasses/Vector3.hpp"

namespace Game::Sound::Classes {
	struct ListenerStruct {
		virtual void SetPosition(const Math::Vector3& Position) {}
		virtual void SetVelocity(const Math::Vector3& Velocity) {}
		virtual void SetOrientation(const Math::Vector3& Look, const Math::Vector3& Up) {}
	};
}

namespace Game::Sound {
    Game::Sound::Classes::ListenerStruct Listener;
}

#endif