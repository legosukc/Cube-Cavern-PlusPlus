#ifndef COLLISIONCONTAINER_HPP
#define COLLISIONCONTAINER_HPP

#include <list>

#include "BaseCollision.hpp"

namespace Game::Collision {
    class CollisionContainer {
        // 1st list = X, 2nd list = Y, 3rd list = Z
        std::list<std::list<std::list<Game::Collision::BaseCollision*>>>
            CollisionBlockmap;

        constexpr static const int BlockSize = 12;

       public:

       inline void AddCollision(Game::Collision::BaseCollision* Collision);
    };
}

#endif