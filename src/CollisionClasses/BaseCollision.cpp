#include "BaseCollision.hpp"

void Game::Collision::BaseCollision::SetPosition(
    const Math::Vector3& Position) {
    this->Position = Position;
    this->RecalculateOccupiedBlocks();
}

void Game::Collision::BaseCollision::SetScale(const Math::Vector3& Scale) {
    this->Scale = Scale;
    this->RecalculateOccupiedBlocks();
}