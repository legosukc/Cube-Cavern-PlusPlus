#include "Texture.hpp"

#include "../../define.h"

#include <cstddef>

#include <SDL3/SDL_video.h>

#ifdef SDL_PLATFORM_VITA
#include <GLES2/gl2.h>
#include "../../GLES2_typedefs.h"
#else
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#endif

namespace Game::Graphics::Texture {

    Game::Graphics::Classes::Texture* (*Create)() = NULL;
    void (*CreateBulk)(size_t CreateAmount,
                       Game::Graphics::Classes::Texture* Textures[]) = NULL;
    void (*Unbind)() = NULL;

    void (*UploadPixelData)(size_t Width,
                            size_t Height,
                            unsigned int Format,
                            unsigned int PixelFormat,
                            const void* PixelData) = NULL;
    void (*SetFilteringMode)(unsigned int FilteringMode) = NULL;

    void (*AttachToFramebuffer)(unsigned int AttachmentType,
                                Game::Graphics::Classes::Texture* Texture) =
        NULL;
}