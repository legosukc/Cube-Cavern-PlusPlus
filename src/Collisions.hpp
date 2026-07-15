#pragma once

#include <vector>

#include <SDL3/SDL_stdinc.h>


namespace {
	namespace Collisions {

		template<Uint8 Resolution>
		struct VoxelCollisionBox;

		template<Uint8 Resolution>
		struct VoxelCollisionBox {
			VoxelCollisionBox<Resolution / 2> a;
		};

		template<>
		struct VoxelCollisionBox<0> {
			std::vector<void*> OverlappingObjects;
		};

		void a() {
			VoxelCollisionBox<64> box;
		}
	}
}

namespace Game::Collision {

	
}