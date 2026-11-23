#pragma once

#include "../MathClasses/Vector3.hpp"

#include "BaseCollision.hpp"


// big meshes will NOT be put into collision container blockmaps
// cause, if they were to move at all, that would update a TON of blockmaps
// there shouldn't be too many huge meshes anyway

// TODO: sort triangles into blockmaps so we don't check every single triangle

namespace Game::Collision {
    class MeshCollision : public Game::Collision::BaseCollision {
       public:
        virtual void SetPosition() {}
        virtual void SetRotation() {}
    };
}