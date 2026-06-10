#pragma once

#include "../../define.h"

#include <SDL3/SDL_opengl_glext.h>

#include "BaseClassDefinitions.hpp"


namespace Game::Graphics::OpenGLFunctions {

	static PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
	static PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;

	static PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
}

namespace Game::Graphics::Classes {

	class Framebuffer : public Game::Graphics::Classes::OpenGLClassBase {
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() {}
		virtual bool IsBound() const {
			return false;
		}
	};

	class Framebuffer_OpenGL : public Framebuffer {
	public:
		virtual ~Framebuffer_OpenGL() override {

			if (this->GLObject == 0) {
#ifdef DEBUG_BUILD
				std::cerr << "Attempted to free a unallocated OpenGL Framebuffer." << std::endl;
				__debugbreak();
#else
				return;
#endif
			}
			Game::Graphics::OpenGLFunctions::glDeleteFramebuffers(1, &this->GLObject);
		}

		virtual void Bind() override {
			Game::Graphics::OpenGLFunctions::glBindFramebuffer(GL_FRAMEBUFFER, this->GLObject);
		}

		virtual bool IsBound() const override {

			// TODO: FIX THIS!!!!
			return true;
		}
	};
}


namespace {


	static Game::Graphics::Classes::Framebuffer* _Framebuffer_Create_OpenGL() {

		Game::Graphics::Classes::Framebuffer_OpenGL* NewFramebuffer = new Game::Graphics::Classes::Framebuffer_OpenGL;
		Game::Graphics::OpenGLFunctions::glGenFramebuffers(1, &NewFramebuffer->GLObject);

		return NewFramebuffer;
	}

	static void _Framebuffer_CreateBulk_OpenGL(size_t CreateAmount, Game::Graphics::Classes::Framebuffer* Textures[]) {
		Game::Graphics::OpenGLFunctions::glGenFramebuffers(CreateAmount, reinterpret_cast<GLuint*>(Textures));
	}

	static void _Framebuffer_Unbind_OpenGL() {
		Game::Graphics::OpenGLFunctions::glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

namespace Game::Graphics::Framebuffer {

	Game::Graphics::Classes::Framebuffer*(*Create)();
	void(*CreateBulk)(size_t CreateAmount, Game::Graphics::Classes::Framebuffer* Textures[]);
	void(*Unbind)();

	inline void Init() {

		if (Game::Graphics::ActiveAPI == Game::Graphics::GraphicsAPIEnum::OpenGL) {

			LOAD_OPENGL_FUNCTION(glGenFramebuffers);
			LOAD_OPENGL_FUNCTION(glDeleteFramebuffers);

			LOAD_OPENGL_FUNCTION(glBindFramebuffer);

			Framebuffer::Create = ::_Framebuffer_Create_OpenGL;
			Framebuffer::CreateBulk = ::_Framebuffer_CreateBulk_OpenGL;

			Framebuffer::Unbind = ::_Framebuffer_Unbind_OpenGL;
		}
	}
}