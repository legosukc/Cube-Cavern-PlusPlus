#include "VertexArray.hpp"

#include <SDL3/SDL_platform_defines.h>
#include <SDL3/SDL_video.h>

namespace Game::Graphics::VertexArray {

    Game::Graphics::Classes::VertexArray* (*Create)() = NULL;
    void (*CreateBulk)(
        size_t CreateAmount,
        Game::Graphics::Classes::VertexArray* VertexArrays[]) = NULL;
    void (*Unbind)() = NULL;

    void (*AddFloatAttribute)(int AttributeIndex, size_t Count) = NULL;
    void (*AddIntAttribute)(int AttributeIndex, size_t Count) = NULL;
}