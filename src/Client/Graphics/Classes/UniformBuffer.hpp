#pragma once

// TODO: make abstraction so uniform buffers work in gles 2.0

#include "../../../define.h"

#include "UniformBuffer/Base_UniformBuffer.hpp"

namespace Game::Graphics::UniformBuffer {

    extern Game::Graphics::Classes::UniformBuffer* (*Create)();
    extern void (*CreateBulk)(
        size_t CreateAmount,
        Game::Graphics::Classes::UniformBuffer* UniformBuffers[]);
    extern void (*Unbind)();
}