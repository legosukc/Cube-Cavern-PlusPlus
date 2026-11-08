#include "Graphics_Client.hpp"

#include <SDL3/SDL_platform_defines.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>

namespace Game::Graphics {

    Uint8 AnisotropicFilteringLevel = 1;

    GraphicsAPIEnum ActiveAPI = GraphicsAPIEnum::None;
}

namespace Game::Graphics {
    BoundObjectsStruct BoundObjects;
    GBufferStruct GBuffer;

    void (*_SetClearColor)(float R, float G, float B, float A);
    void (*_ClearBitfields)(unsigned int Bitfields);
}

void Game::Graphics::_misc::_Namespace_Buffer::CopyToBuffer(
    _Enums_BindTargets::IntType BindTarget,
    size_t Size,
    const void* Data,
    _Enums_AllocationTypes::IntType AllocateFor) {}

void Game::Graphics::_misc::_Namespace_Buffer::CopyToBufferPointer(
    _Enums_BindTargets::IntType BindTarget,
    int Offset,
    size_t Size,
    const void* Data) {}

void Game::Graphics::_misc::_Namespace_Buffer::AllocateBuffer(
    _Enums_BindTargets::IntType BindTarget,
    size_t Size,
    _Enums_AllocationTypes::IntType AllocateFor) {}

namespace Game::Graphics {
    Game::Graphics::_misc::_Enums_BindTargets BindTargets;
    Game::Graphics::_misc::_Enums_AllocationTypes AllocationTypes;
    Game::Graphics::_misc::_Enums_BufferBitfields BufferBitfields;
}

void Game::Graphics::Draw() {}