#include "Base_Program.hpp"

Game::Graphics::Classes::Program::~Program() = default;

GLint Game::Graphics::Classes::Program::GetUniformLocation(
    const char* UniformName) const {
    return -1;
}

bool Game::Graphics::Classes::Program::Link(bool Cache) {
    return false;
}

void Game::Graphics::Classes::Program::Use() {}