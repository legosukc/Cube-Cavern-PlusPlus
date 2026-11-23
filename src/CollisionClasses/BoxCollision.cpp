#include "BaseCollision.hpp"
#include "CollisionContainer.hpp"

#include "BoxCollision.hpp"
#include <vector>

// not sure if this works
void Game::Collision::BoxCollision::RecalculateOccupiedBlocks() {

    for (Uint32 i = 0; i < this->OccupiedBlocksCount; ++i) {
        this->OccupiedBlocksArray[i] = NULL;
    }

    const Math::IVector3 BlockmapPosition = static_cast<Math::IVector3>(
        (this->Position / static_cast<float>(this->_ParentContainer->BlockSize))
            .Round());

    const Math::IVector3 OccupingBlockmaps =
        static_cast<Math::IVector3>(
            (this->Scale /
             static_cast<float>(this->_ParentContainer->BlockSize))
                .Round());

                std::vector<Game::Collision::BaseCollision*>* BlockmapCollisions;
    const Math::IVector3 Step = OccupingBlockmaps / OccupingBlockmaps.Abs();
    Sint32 Z, Y, X;
    for (X = OccupingBlockmaps.X/-2; X == OccupingBlockmaps.X/2; X += Step.X) {
        for (Y = -OccupingBlockmaps.Y/-2; Y == OccupingBlockmaps.Y/2; Y += Step.Y) {
            for (Z = -OccupingBlockmaps.Z/-2; Z == OccupingBlockmaps.Z/2;
                 Z += Step.Z) {
                BlockmapCollisions = &(*this->_ParentContainer)[Math::IVector3(X, Y, Z)];
                BlockmapCollisions->push_back(this);
                this->OccupiedBlocksArray = &BlockmapCollisions->back();
            }
        }
    }
}
/*
void Game::Collision::BoxCollision::SetPosition(const Math::Vector3& Position) {
}

void Game::Collision::BoxCollision::SetScale(const Math::Vector3& Scale) {}
*/