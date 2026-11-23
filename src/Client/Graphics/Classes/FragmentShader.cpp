#include "FragmentShader.hpp"
namespace Game::Graphics::FragmentShader {

    Game::Graphics::Classes::FragmentShader* (*Create)() = NULL;
    void (*CreateBulk)(
        size_t CreateAmount,
        Game::Graphics::Classes::FragmentShader* FragmentShaders[]) = NULL;
}