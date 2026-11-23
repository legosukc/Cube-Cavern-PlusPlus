#pragma once

#include "VertexBuffer/Base_VertexBuffer.hpp"

namespace Game::Graphics::VertexBuffer {

    extern Game::Graphics::Classes::VertexBuffer* (*Create)();
    extern void (*CreateBulk)(
        size_t CreateAmount,
        Game::Graphics::Classes::VertexBuffer* VertexBuffers[]);
    extern void (*Unbind)();
}