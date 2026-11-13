#include "Collisions.hpp"

#include <SDL3/SDL_stdinc.h>

#include "CollisionClasses/BaseCollision.hpp"
#include "CollisionClasses/BoxCollision.hpp"
#include "CollisionClasses/SmallMeshCollision.hpp"
#include "CollisionClasses/SphereCollision.hpp"

#include "CollisionClasses/CollisionContainer.hpp"

namespace Game::Collision {
    Game::Collision::CollisionContainer DefaultContainer;
}