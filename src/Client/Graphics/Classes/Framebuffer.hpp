#pragma once

#include "../../../define.h"

#include "Framebuffer/Base_Framebuffer.hpp"

namespace Game::Graphics::Framebuffer {

    extern Game::Graphics::Classes::Framebuffer* (*Create)();
    extern void (*CreateBulk)(size_t CreateAmount,
                              Game::Graphics::Classes::Framebuffer* Textures[]);
    extern void (*Unbind)();
}