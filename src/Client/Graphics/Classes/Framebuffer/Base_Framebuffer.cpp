#include "Base_Framebuffer.hpp"

Game::Graphics::Classes::Framebuffer::~Framebuffer() = default;

void Game::Graphics::Classes::Framebuffer::Bind() {}
bool Game::Graphics::Classes::Framebuffer::IsBound() const {
    return false;
}