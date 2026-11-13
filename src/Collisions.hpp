#pragma once

#include <SDL3/SDL_stdinc.h>

#include "CollisionClasses/BaseCollision.hpp"
#include "CollisionClasses/BoxCollision.hpp"
#include "CollisionClasses/SphereCollision.hpp"
#include "CollisionClasses/SmallMeshCollision.hpp"

#include "CollisionClasses/CollisionContainer.hpp"

namespace Game::Collision {

    inline void Init();
    inline void Quit();

    extern Game::Collision::CollisionContainer DefaultContainer;
}


void Game::Collision::Init() {}

void Game::Collision::Quit() {}