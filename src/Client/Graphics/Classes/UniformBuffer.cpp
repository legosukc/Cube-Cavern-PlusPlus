#include "UniformBuffer.hpp"

namespace Game::Graphics::UniformBuffer {

    Game::Graphics::Classes::UniformBuffer* (*Create)() = NULL;
    void (*CreateBulk)(
        size_t CreateAmount,
        Game::Graphics::Classes::UniformBuffer* UniformBuffers[]) = NULL;
    void (*Unbind)() = NULL;
}