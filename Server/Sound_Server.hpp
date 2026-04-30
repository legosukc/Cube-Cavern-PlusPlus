#pragma once

namespace Game::Sound {

	namespace Listener {
		void SetPosition(const Math::Vector3& Position);
		void SetVelocity(const Math::Vector3& Velocity);
		void SetOrientation(const Math::Vector3& Look, const Math::Vector3& Up);
	}

	void PlaySound();
}