#pragma once

#include "../MathClasses/Vector3.hpp"

#include "BaseCollision.hpp"

namespace Game::Collision {
    class BoxCollision : public Game::Collision::BaseCollision {
        virtual void RecalculateOccupiedBlocks() override;
       public:
        //virtual void SetPosition(const Math::Vector3& Position) override;
        //virtual void SetScale(const Math::Vector3& Scale) override;
    };
}