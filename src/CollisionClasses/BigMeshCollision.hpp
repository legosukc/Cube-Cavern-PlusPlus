#pragma once

#include "../MathClasses/Vector3.hpp"

#include "BaseCollision.hpp"


// TODO: sort triangles into blockmaps so we don't check every single triangle

namespace Game::Collision {
    class MeshCollision : public Game::Collision::BaseCollision {
       public:
        virtual void SetPosition() {}
        virtual void SetRotation() {}
    };
}