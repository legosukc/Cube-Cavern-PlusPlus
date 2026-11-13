#include "Base_BufferBase.hpp"

Game::Graphics::Classes::BufferBase::~BufferBase() = default;

void Game::Graphics::Classes::BufferBase::Bind() {}
bool Game::Graphics::Classes::BufferBase::IsBound() const {
    return false;
}