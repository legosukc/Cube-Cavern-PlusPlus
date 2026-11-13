#include "N3DS_Texture.hpp"

namespace Game::Graphics::Texture::Wrappers {
    Game::Graphics::Classes::Texture_N3DS* BoundTexture_N3DS = NULL;
}

Game::Graphics::Classes::Texture_N3DS::~Texture_N3DS() {
    C3D_TexDelete(&this->TexObj);
}

void Game::Graphics::Classes::Texture_N3DS::Bind(int BindIndex) {
    C3D_TexBind(BindIndex, &this->TexObj);
}

bool Game::Graphics::Classes::Texture_N3DS::IsBound() const {
    return Game::Graphics::Texture::Wrappers::BoundTexture_N3DS == this;
}

Game::Graphics::Classes::Texture*
    Game::Graphics::Texture::Wrappers::Create_N3DS() {
    Game::Graphics::Classes::Texture_N3DS* NewTexture =
        new Game::Graphics::Classes::Texture_N3DS;

    return reinterpret_cast<Game::Graphics::Classes::Texture*>(NewTexture);
}

void Game::Graphics::Texture::Wrappers::CreateBulk_N3DS(
    size_t CreateAmount,
    Game::Graphics::Classes::Texture* Textures[]) {
    for (int i = 0; i < CreateAmount; ++i) {
        Textures[i] = reinterpret_cast<Game::Graphics::Classes::Texture*>(
            new Game::Graphics::Classes::Texture_N3DS);
    }
}

void Game::Graphics::Texture::Wrappers::Unbind_N3DS() {
    // Game::Graphics::OpenGLFunctions::glBindTexture(GL_TEXTURE_2D, 0);
}

void Game::Graphics::Texture::Wrappers::UploadPixelData_N3DS(
    size_t Width,
    size_t Height,
    unsigned int Format,
    unsigned int PixelFormat,
    const void* PixelData) {
    // TODO: idk
    // C3D_TexInit(&Wrappers::BoundTexture_N3DS->TexObj, Width, Height,
    // GPU_TEXCOLOR format);
}

void Game::Graphics::Texture::Wrappers::SetFilteringMode_N3DS(
    unsigned int FilteringMode) {
    // TODO: fix filtering modes
    C3D_TexSetFilter(&Wrappers::BoundTexture_N3DS->TexObj, FilteringMode,
                     FilteringMode);
}

void Game::Graphics::Texture::Wrappers::AttachToFramebuffer_N3DS(
    unsigned int AttachmentType,
    Game::Graphics::Classes::Texture* Texture) {
    /*
Game::Graphics::OpenGLFunctions::glFramebufferTexture2D(
    GL_FRAMEBUFFER, AttachmentType, GL_TEXTURE_2D,
    reinterpret_cast<Game::Graphics::Classes::Texture_OpenGL*>(Texture)
        ->GLObject,
    0);
    */
}