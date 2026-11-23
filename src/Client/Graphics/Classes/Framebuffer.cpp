#include "Framebuffer.hpp"

namespace Game::Graphics::Framebuffer {

    Game::Graphics::Classes::Framebuffer* (*Create)() = NULL;
    void (*CreateBulk)(size_t CreateAmount,
                              Game::Graphics::Classes::Framebuffer* Textures[]) = NULL;
    void (*Unbind)() = NULL;
}