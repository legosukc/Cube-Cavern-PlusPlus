
#include "BaseClassDefinitions.hpp"

constexpr bool Game::Graphics::Classes::OpenGLClassBase::operator==(
    const Game::Graphics::Classes::OpenGLClassBase B) const {
    return this->GLObject == B.GLObject;
}