#ifndef COLLISIONS_BASECOLLISION_HPP
#define COLLISIONS_BASECOLLISION_HPP 1

#include "../MathClasses/Vector3.hpp"

namespace Game::Collision {
    class CollisionContainer;
}

namespace Game::Collision {
    class BaseCollision {
       protected:
        Math::Vector3 Position;
        Math::Vector3 Rotation;

        Game::Collision::CollisionContainer* _ParentContainer = NULL;

        virtual void RecalculateOccupiedBlocks() = 0;

       public:
        BaseCollision(Game::Collision::CollisionContainer& ParentContainer) : _ParentContainer(&ParentContainer) {}

        virtual void SetPosition() = 0;
        virtual void SetRotation() = 0;

        inline Math::Vector3 GetPosition() const { return this->Position; }
        inline Math::Vector3 GetRotation() const { return this->Rotation; }

        friend Game::Collision::CollisionContainer;
    };
}

#endif