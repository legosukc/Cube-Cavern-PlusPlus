#pragma once

#include "../../../define.h"

#include "Texture/Base_Texture.hpp"

namespace Game::Graphics::Texture {

    extern Game::Graphics::Classes::Texture* (*Create)();
    extern void (*CreateBulk)(size_t CreateAmount,
                              Game::Graphics::Classes::Texture* Textures[]);
    extern void (*Unbind)();

    extern void (*UploadPixelData)(size_t Width,
                                   size_t Height,
                                   unsigned int Format,
                                   unsigned int PixelFormat,
                                   const void* PixelData);
    extern void (*SetFilteringMode)(unsigned int FilteringMode);

    extern void (*AttachToFramebuffer)(
        unsigned int AttachmentType,
        Game::Graphics::Classes::Texture* Texture);
}