#pragma once

#include "../../define.h"

#include <SDL3/SDL_opengl_glext.h>

#include "BaseClassDefinitions.hpp"


namespace Game::Graphics::OpenGLFunctions {

	static PFNGLGENTEXTURESPROC glGenTextures;
	static PFNGLDELETETEXTURESPROC glDeleteTextures;

	static PFNGLACTIVETEXTUREPROC glActiveTexture;
	static PFNGLBINDTEXTUREPROC glBindTexture;

	static PFNGLTEXIMAGE2DPROC glTexImage2D;
	static PFNGLTEXPARAMETERIPROC glTexParameteri;

	static PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
}

namespace Game::Graphics::Classes {

	class Texture : public Game::Graphics::Classes::OpenGLClassBase {
	public:
		virtual ~Texture() = default;

		virtual void Bind() {}
		virtual bool IsBound() const {
			return false;
		}
	};

	class Texture_OpenGL : public Texture {
	public:
		virtual ~Texture_OpenGL() override {

			if (this->GLObject == 0) {
#ifdef DEBUG_BUILD
				std::cerr << "Attempted to free a unallocated OpenGL Texture." << std::endl;
				__debugbreak();
#else
				return;
#endif
			}
			Game::Graphics::OpenGLFunctions::glDeleteTextures(1, &this->GLObject);
		}

		virtual void Bind() override {
			Game::Graphics::OpenGLFunctions::glBindTexture(GL_TEXTURE_2D, this->GLObject);
		}

		virtual bool IsBound() const override {

			// TODO: FIX THIS!!!!
			return true;
		}
	};
}


namespace {


	static Game::Graphics::Classes::Texture* _Texture_Create_OpenGL() {

		Game::Graphics::Classes::Texture_OpenGL* NewTexture = new Game::Graphics::Classes::Texture_OpenGL;
		Game::Graphics::OpenGLFunctions::glGenTextures(1, &NewTexture->GLObject);

		return reinterpret_cast<Game::Graphics::Classes::Texture*>(NewTexture);
	}

	static void _Texture_CreateBulk_OpenGL(size_t CreateAmount, Game::Graphics::Classes::Texture* Textures[]) {

		GLuint* TextureObjects = SDL_stack_alloc(GLuint, CreateAmount);
		Game::Graphics::Classes::Texture_OpenGL* TempTexture;

		Game::Graphics::OpenGLFunctions::glGenTextures(CreateAmount, TextureObjects);
		for (int i = 0; i < CreateAmount; ++i) {

			TempTexture = new Game::Graphics::Classes::Texture_OpenGL;
			TempTexture->GLObject = TextureObjects[i];
			Textures[i] = TempTexture;
		}
		SDL_stack_free(TextureObjects);
	}

	static void _Texture_Unbind_OpenGL() {
		Game::Graphics::OpenGLFunctions::glBindTexture(GL_TEXTURE_2D, 0);
	}

	static void _Texture_UploadPixelData(size_t Width, size_t Height, unsigned int Format, unsigned int PixelFormat, const void* PixelData) {
		Game::Graphics::OpenGLFunctions::glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, PixelFormat, PixelData);
	}

	static void _Texture_SetFilteringMode(unsigned int FilteringMode) {
		Game::Graphics::OpenGLFunctions::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilteringMode);
		Game::Graphics::OpenGLFunctions::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilteringMode);
	}

	static void _Texture_AttachToFramebuffer(unsigned int AttachmentType, Game::Graphics::Classes::Texture* Texture) {
		Game::Graphics::OpenGLFunctions::glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			AttachmentType,
			GL_TEXTURE_2D,
			reinterpret_cast<Game::Graphics::Classes::Texture_OpenGL*>(Texture)->GLObject, 0
		);
	}
}

namespace Game::Graphics::Texture {

	Game::Graphics::Classes::Texture*(*Create)();
	void(*CreateBulk)(size_t CreateAmount, Game::Graphics::Classes::Texture* Textures[]);
	void(*Unbind)();

	void(*UploadPixelData)(size_t Width, size_t Height, unsigned int Format, unsigned int PixelFormat, const void* PixelData);
	void(*SetFilteringMode)(unsigned int FilteringMode);
	
	void(*AttachToFramebuffer)(unsigned int AttachmentType, Game::Graphics::Classes::Texture* Texture);
	
	inline void Init() {

		if (Game::Graphics::ActiveAPI == Game::Graphics::GraphicsAPIEnum::OpenGL) {

			LOAD_OPENGL_FUNCTION(glGenTextures);
			LOAD_OPENGL_FUNCTION(glDeleteTextures);

			LOAD_OPENGL_FUNCTION(glActiveTexture);
			LOAD_OPENGL_FUNCTION(glBindTexture);

			LOAD_OPENGL_FUNCTION(glTexImage2D);
			LOAD_OPENGL_FUNCTION(glTexParameteri);

			LOAD_OPENGL_FUNCTION(glFramebufferTexture2D);

			Texture::Create = ::_Texture_Create_OpenGL;
			Texture::CreateBulk = ::_Texture_CreateBulk_OpenGL;

			Texture::Unbind = ::_Texture_Unbind_OpenGL;

			Texture::UploadPixelData = ::_Texture_UploadPixelData;
			Texture::SetFilteringMode = ::_Texture_SetFilteringMode;

			Texture::AttachToFramebuffer = ::_Texture_AttachToFramebuffer;
		}
	}
}