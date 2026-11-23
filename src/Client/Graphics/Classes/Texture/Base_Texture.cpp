#include "Base_Texture.hpp"

Game::Graphics::Classes::Texture::~Texture() = default;

void Game::Graphics::Classes::Texture::Bind(int BindIndex) {}

bool Game::Graphics::Classes::Texture::IsBound() const {
    return false;
}