#ifndef TEXTURE_CLASS
#define TEXTURE_CLASS

#include <SDL2/SDL_surface.h>

#include <glad/glad.h>

#include <memory>
#include <map>



namespace OpenGL {
    namespace Classes {
        class Texture;
    }
}


namespace OpenGL {
    namespace Texture2D {


        void LoadImageAndBindTexture(OpenGL::Classes::Texture& Texture, const char* TexturePath, SDL_PixelFormatEnum SDLPixelFormat, GLenum Format, GLenum InternalFormat, GLenum ByteFormat);
        inline void LoadImageAndBindTexture(OpenGL::Classes::Texture& Texture, const char* TexturePath);


        // Sets a Texture Parameter on the bound 2D Texture.
        inline void SetParameteri(GLenum Parameter, GLint SetTo);


        inline void UploadTextureData(GLenum InternalFormat, GLsizei Width, GLsizei Height, GLenum Format, GLenum TextureByteType, const void* Pixels);
        inline void AllocateTexture(GLenum InternalFormat, GLsizei Width, GLsizei Height, GLenum Format, GLenum TextureByteType);

        inline void GenerateMipmaps();


        inline void SetActiveTexture(GLenum TextureIndex);
        inline void UnbindActiveTexture();


        void AttachTexture(GLenum Target, GLenum AttachmentType, Classes::Texture& Texture);
        inline void AttachTexture(GLenum AttachmentType, Classes::Texture& Texture);

        void SetTextureFilter(GLenum FilterEnum);
        void SetTextureRepeatMode(GLenum RepeatType);


        inline void AllocateAs2DTextureArray(GLenum Format, GLsizei Width, GLsizei Height, GLsizei Layers);
        inline void Add2DTextureArrayLayer(GLint Layer, GLsizei Width, GLsizei Height, GLenum Format, GLenum ByteFormat, const void* Pixels);

        inline void ReadPixels(void* Buffer, GLsizei Width, GLsizei Height, GLenum Format, GLenum ByteType);
        std::unique_ptr<char[]> ReadPixels(GLsizei Width, GLsizei Height, GLenum Format, GLenum ByteType);
    };
}

namespace OpenGL {
    namespace Classes {

        class Texture {

            GLuint GLObject;
#ifdef DEBUG
            mutable bool Initialized = false;
#endif
        public:
            inline Texture() = default;
            inline ~Texture();

            inline void Bind(GLenum TextureBinding = GL_TEXTURE_2D);

            inline void Create();
            static Texture CreateConstruct();

            friend void OpenGL::Texture2D::AttachTexture(GLenum Target, GLenum AttachmentType, OpenGL::Classes::Texture& Texture);
        };
    }
}


OpenGL::Classes::Texture::~Texture() {

#ifdef DEBUG
    if (!this->Initialized) {
        std::cerr << "You fucked up. You forgot to call 'Create' on a Texture and its deconstructor was called. Shame on you, bastard." << std::endl;
        __debugbreak();
    }
#endif

    glDeleteTextures(1, &this->GLObject);
}


void OpenGL::Classes::Texture::Bind(GLenum TextureBinding) {

#ifdef DEBUG
    if (!this->Initialized) {
        std::cerr << "You fucked up. You forgot to call 'Create' on a Texture and 'Bind' was called on it. Shame on you, bastard." << std::endl;
        __debugbreak();
    }
#endif
    glBindTexture(TextureBinding, this->GLObject);
}

void OpenGL::Classes::Texture::Create() {
    glGenTextures(1, &this->GLObject);
#ifdef DEBUG
    this->Initialized = true;
#endif
}

OpenGL::Classes::Texture OpenGL::Classes::Texture::CreateConstruct() {
    OpenGL::Classes::Texture Texture;
    Texture.Create();
    return Texture;
}


namespace OpenGL {
    namespace MiscClasses {

        class TextureProperties {
        public:
            inline TextureProperties();
            TextureProperties(const OpenGL::Classes::Texture& Texture);

            GLsizei Width, Height;
        };
    }
}

OpenGL::MiscClasses::TextureProperties::TextureProperties() {}

OpenGL::MiscClasses::TextureProperties::TextureProperties(const OpenGL::Classes::Texture& Texture) {

    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &this->Width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &this->Height);
}


// Load a .bmp image into a Texture, and bind said texture.
void OpenGL::Texture2D::LoadImageAndBindTexture(OpenGL::Classes::Texture& Texture, const char* TexturePath, SDL_PixelFormatEnum SDLPixelFormat, GLenum Format, GLenum InternalFormat, GLenum ByteFormat) {

    

    SDL_Surface* ConvertedSurface;
    {
        SDL_Surface* Surface = SDL_LoadBMP(TexturePath);
        if (SDLPixelFormat != 0) {
            ConvertedSurface = SDL_ConvertSurfaceFormat(Surface, SDLPixelFormat, 0);
            SDL_FreeSurface(Surface);
        } else {
            ConvertedSurface = Surface;
        }
    }

    Texture.Bind();
    OpenGL::Texture2D::UploadTextureData(Format, ConvertedSurface->w, ConvertedSurface->h, InternalFormat, ByteFormat, ConvertedSurface->pixels);

    SDL_FreeSurface(ConvertedSurface);
}

void OpenGL::Texture2D::LoadImageAndBindTexture(OpenGL::Classes::Texture& Texture, const char* TexturePath) {
    OpenGL::Texture2D::LoadImageAndBindTexture(Texture, TexturePath, SDL_PIXELFORMAT_RGB24, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
}


// Sets a Texture Parameter on the bound 2D Texture.
void OpenGL::Texture2D::SetParameteri(GLenum Parameter, GLint SetTo) {
    glTexParameteri(GL_TEXTURE_2D, Parameter, SetTo);
}


void OpenGL::Texture2D::UploadTextureData(GLenum InternalFormat, GLsizei Width, GLsizei Height, GLenum Format, GLenum TextureByteType, const void* Pixels) {
    glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, Width, Height, 0, Format, TextureByteType, Pixels);
}

void OpenGL::Texture2D::AllocateTexture(GLenum InternalFormat, GLsizei Width, GLsizei Height, GLenum Format, GLenum TextureByteType) {
    OpenGL::Texture2D::UploadTextureData(InternalFormat, Width, Height, Format, TextureByteType, nullptr);
}


void OpenGL::Texture2D::GenerateMipmaps() {
    glGenerateMipmap(GL_TEXTURE_2D);
}


void OpenGL::Texture2D::SetActiveTexture(GLenum TextureIndex) {
    glActiveTexture(TextureIndex);
}

void OpenGL::Texture2D::UnbindActiveTexture() {
    glBindTexture(GL_TEXTURE_2D, 0);
}


void OpenGL::Texture2D::AttachTexture(GLenum Target, GLenum AttachmentType, OpenGL::Classes::Texture& Texture) {
    glFramebufferTexture2D(Target, AttachmentType, GL_TEXTURE_2D, Texture.GLObject, 0);
}

void OpenGL::Texture2D::AttachTexture(GLenum AttachmentType, OpenGL::Classes::Texture& Texture) {
    OpenGL::Texture2D::AttachTexture(GL_FRAMEBUFFER, AttachmentType, Texture);
}


void OpenGL::Texture2D::SetTextureFilter(GLenum FilterEnum) {
    OpenGL::Texture2D::SetParameteri(GL_TEXTURE_MIN_FILTER, FilterEnum);
    OpenGL::Texture2D::SetParameteri(GL_TEXTURE_MAG_FILTER, FilterEnum);
}

void OpenGL::Texture2D::SetTextureRepeatMode(GLenum RepeatType) {
    OpenGL::Texture2D::SetParameteri(GL_TEXTURE_WRAP_S, RepeatType);
    OpenGL::Texture2D::SetParameteri(GL_TEXTURE_WRAP_T, RepeatType);
}


void OpenGL::Texture2D::AllocateAs2DTextureArray(GLenum Format, GLsizei Width, GLsizei Height, GLsizei Layers) {
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 0, Format, Width, Height, Layers);
}

void OpenGL::Texture2D::Add2DTextureArrayLayer(GLint Layer, GLsizei Width, GLsizei Height, GLenum Format, GLenum ByteFormat, const void* Pixels) {
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, Layer, Width, Height, 1, Format, ByteFormat, Pixels);
}

void OpenGL::Texture2D::ReadPixels(void* Buffer, GLsizei Width, GLsizei Height, GLenum Format, GLenum ByteType) {
    glReadPixels(0, 0, Width, Height, Format, ByteType, Buffer);
}

std::unique_ptr<char[]> OpenGL::Texture2D::ReadPixels(GLsizei Width, GLsizei Height, GLenum Format, GLenum ByteType) {

    std::unique_ptr<char[]> Pixels = std::make_unique<char[]>(Width * Height);
    OpenGL::Texture2D::ReadPixels(Pixels.get(), Width, Height, Format, ByteType);
    return Pixels;
}

#endif