#include "VertexShader.hpp"

namespace Game::Graphics::VertexShader {

    Game::Graphics::Classes::VertexShader* (*Create)() = NULL;
    void (*CreateBulk)(size_t CreateAmount,
                       Game::Graphics::Classes::VertexShader* VertexShaders[]) =
        NULL;
}