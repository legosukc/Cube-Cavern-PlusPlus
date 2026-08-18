#ifndef COLLISIONS_HPP
#define COLLISIONS_HPP 1

#include <SDL3/SDL_stdinc.h>

#include "CollisionClasses/BaseCollision.hpp"
#include "CollisionClasses/BoxCollision.hpp"
#include "CollisionClasses/SphereCollision.hpp"
#include "CollisionClasses/MeshCollision.hpp"

#include "CollisionClasses/CollisionContainer.hpp"

namespace Game::Collision {

    inline void Init();
    inline void Quit();
    Game::Collision::CollisionContainer DefaultContainer;
}


void Game::Collision::Init() {}

void Game::Collision::Quit() {}

#endif