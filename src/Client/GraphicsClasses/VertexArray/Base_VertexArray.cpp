#include "Base_VertexArray.hpp"

Game::Graphics::Classes::VertexArray::~VertexArray() = default;

void Game::Graphics::Classes::VertexArray::Bind() {}

bool Game::Graphics::Classes::VertexArray::IsBound() const {
    return false;
}