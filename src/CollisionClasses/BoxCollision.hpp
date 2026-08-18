#ifndef COLLISIONS_BOXCOLLISION_HPP
#define COLLISIONS_BOXCOLLISION_HPP 1

#include "../MathClasses/Vector3.hpp"

#include "BaseCollision.hpp"

namespace Game::Collision {
    class BoxCollision : public Game::Collision::BaseCollision {
       public:
        virtual void SetPosition() override {

        }
        virtual void SetRotation() override {
            
        }
    };
}

#endif