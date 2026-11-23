#include "IndexBuffer.hpp"

namespace Game::Graphics::IndexBuffer {

    Game::Graphics::Classes::IndexBuffer* (*Create)() = NULL;
    void (*CreateBulk)(
        size_t CreateAmount,
        Game::Graphics::Classes::IndexBuffer* IndexBuffers[]) = NULL;
    void (*Unbind)() = NULL;
}