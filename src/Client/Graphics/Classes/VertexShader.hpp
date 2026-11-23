#pragma once

#include "VertexShader/Base_VertexShader.hpp"

namespace Game::Graphics::VertexShader {

    extern Game::Graphics::Classes::VertexShader* (*Create)();
    extern void (*CreateBulk)(size_t CreateAmount,
                       Game::Graphics::Classes::VertexShader* VertexShaders[]);
}