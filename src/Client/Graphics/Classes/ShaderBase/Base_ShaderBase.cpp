#include "Base_ShaderBase.hpp"

Game::Graphics::Classes::ShaderBase::~ShaderBase() = default;

bool Game::Graphics::Classes::ShaderBase::SetShaderSource(const char* Source) {
    return false;
}

bool Game::Graphics::Classes::ShaderBase::Compile() {
    return false;
}