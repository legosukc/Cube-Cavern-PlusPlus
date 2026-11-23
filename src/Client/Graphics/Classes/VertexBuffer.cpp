#include "VertexBuffer.hpp"
namespace Game::Graphics::VertexBuffer {

    Game::Graphics::Classes::VertexBuffer* (*Create)() = NULL;
    void (*CreateBulk)(
        size_t CreateAmount,
        Game::Graphics::Classes::VertexBuffer* VertexBuffers[]) = NULL;
    void (*Unbind)() = NULL;
}