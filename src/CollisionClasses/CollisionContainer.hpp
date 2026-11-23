#pragma once

#include <SDL3/SDL_stdinc.h>

#include <unordered_map>
#include <vector>

#include "BaseCollision.hpp"

namespace Game::Collision {
    class CollisionContainer {
        // TODO: implement sorting bound boxes into blockmaps
        // so we don't have to check againstevery single bounding box

        constexpr static Uint64 HashIVector3(
            const Math::IVector3& IVec3) noexcept {
            Uint64 h = static_cast<Uint64>(IVec3.X) * 0x9E3779B185EBCA87ULL;
            h ^= static_cast<Uint64>(IVec3.Y) * 0xC2B2AE3D27D4EB4FULL;
            h ^= static_cast<Uint64>(IVec3.Z) * 0x165667B19E3779F9ULL;
            h ^= h >> 30;
            h *= 0xBF58476D1CE4E5B9ULL;
            h ^= h >> 27;
            h *= 0x94D049BB133111EBULL;
            h ^= h >> 31;
            return h;
        }

        std::unordered_map<Uint64, std::vector<Game::Collision::BaseCollision*>>
            BlockmapStart;

        friend Game::Collision::BaseCollision;

       public:
        CollisionContainer() = default;
        ~CollisionContainer() = default;

        constexpr static const int BlockSize = 16;

        constexpr std::vector<Game::Collision::BaseCollision*>& operator[](
            const Math::Vector3& Vec3) {
            return this->BlockmapStart[HashIVector3(static_cast<Math::IVector3>(
                (Vec3 / static_cast<float>(this->BlockSize)).Round()))];
        }

        inline void AddCollision(Game::Collision::BaseCollision* Collision) {
            Collision->_ParentContainer = this;
            Collision->RecalculateOccupiedBlocks();
        }
    };
}