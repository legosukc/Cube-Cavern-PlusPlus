#pragma once

#include "../../MathClasses/Vector3.hpp"

namespace Game::Sound::Classes {
	struct ListenerStruct {
		virtual void SetPosition(const Math::Vector3& Position) {}
		virtual void SetVelocity(const Math::Vector3& Velocity) {}
		virtual void SetOrientation(const Math::Vector3& Look, const Math::Vector3& Up) {}
	};
}