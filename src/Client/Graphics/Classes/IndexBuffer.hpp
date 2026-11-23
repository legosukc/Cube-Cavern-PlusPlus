#pragma once

#include "IndexBuffer/Base_IndexBuffer.hpp"

namespace Game::Graphics::IndexBuffer {

    extern Game::Graphics::Classes::IndexBuffer* (*Create)();
    extern void (*CreateBulk)(
        size_t CreateAmount,
        Game::Graphics::Classes::IndexBuffer* IndexBuffers[]);
    extern void (*Unbind)();
}