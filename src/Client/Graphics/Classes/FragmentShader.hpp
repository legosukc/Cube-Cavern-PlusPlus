#pragma once

#include "../../../define.h"

#include "FragmentShader/Base_FragmentShader.hpp"

namespace Game::Graphics::FragmentShader {

    extern Game::Graphics::Classes::FragmentShader* (*Create)();
    extern void (*CreateBulk)(
        size_t CreateAmount,
        Game::Graphics::Classes::FragmentShader* FragmentShaders[]);
}