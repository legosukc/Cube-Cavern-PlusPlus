#ifndef COLLISIONS_MESHCOLLISION_HPP
#define COLLISIONS_MESHCOLLISION_HPP 1

#include "../MathClasses/Vector3.hpp"

#include "BaseCollision.hpp"

namespace Game::Collision {
    class MeshCollision : public Game::Collision::BaseCollision {
       public:
        virtual void SetPosition() {}
        virtual void SetRotation() {}
    };
}

#endif