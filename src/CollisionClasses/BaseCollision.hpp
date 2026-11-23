#pragma once

#include "../MathClasses/Vector3.hpp"

namespace Game::Collision {
    class CollisionContainer;
}

namespace Game::Collision {
    class BaseCollision {
       protected:
        Math::Vector3 Position;
        Math::Vector3 Scale;

        Game::Collision::CollisionContainer* _ParentContainer = NULL;

        Game::Collision::BaseCollision** OccupiedBlocksArray = NULL;
        Uint32 OccupiedBlocksCount = 0;

        virtual void RecalculateOccupiedBlocks() = 0;

       public:
        inline BaseCollision(
            Game::Collision::CollisionContainer& ParentContainer)
            : _ParentContainer(&ParentContainer) {}

        virtual void SetPosition(const Math::Vector3& Position);
        virtual void SetScale(const Math::Vector3& Scale);

        inline const Math::Vector3& GetPosition() const { return this->Position; }
        inline const Math::Vector3& GetScale() const { return this->Scale; }

        friend Game::Collision::CollisionContainer;
    };
}