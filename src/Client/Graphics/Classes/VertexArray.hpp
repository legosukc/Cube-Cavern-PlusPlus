#pragma once

#include "../../../define.h"

#include "VertexArray/Base_VertexArray.hpp"

namespace Game::Graphics::VertexArray {

    extern Game::Graphics::Classes::VertexArray* (*Create)();
    extern void (*CreateBulk)(
        size_t CreateAmount,
        Game::Graphics::Classes::VertexArray* VertexArrays[]);
    extern void (*Unbind)();

    extern void (*AddFloatAttribute)(int AttributeIndex,
                                     size_t Count,
                                     size_t Stride);
    extern void (*AddIntAttribute)(int AttributeIndex,
                                   size_t Count,
                                   size_t Stride);
}