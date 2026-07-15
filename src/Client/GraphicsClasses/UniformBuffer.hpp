#pragma once

#include <SDL3/SDL_opengl_glext.h>

#include "BaseClassDefinitions.hpp"


namespace Game::Graphics::OpenGLFunctions {

	static PFNGLBINDBUFFERBASEPROC glBindBufferBase;
	static PFNGLBINDBUFFERRANGEPROC glBindBufferRange;
}

namespace Game::Graphics::Classes {

	class UniformBuffer : virtual public Game::Graphics::Classes::BufferBase {
	protected:
		UniformBuffer() = default;
		friend Game::Graphics::Classes::BufferBase;

	public:
		virtual void BindBase(GLuint Index) {}
	};

	class UniformBuffer_OpenGL : public UniformBuffer, public Game::Graphics::Classes::BufferBase_OpenGL<GL_UNIFORM_BUFFER> {
	public:

		virtual void BindBase(GLuint Index) override {
			Game::Graphics::OpenGLFunctions::glBindBufferBase(GL_UNIFORM_BUFFER, Index, this->GLObject);
		}

		virtual bool IsBound() const override {
			// TODO: FIX THIS!!!!
			return true;
		}
	};
}


namespace Game::Graphics::UniformBuffer {

	Game::Graphics::Classes::UniformBuffer*(*Create)();
	void(*CreateBulk)(size_t CreateAmount, Game::Graphics::Classes::UniformBuffer* UniformBuffers[]);
	void(*Unbind)();

	inline void Init() {

		if (Game::Graphics::ActiveAPI == Game::Graphics::GraphicsAPIEnum::OpenGL) {

			LOAD_OPENGL_FUNCTION(glBindBufferBase);
			LOAD_OPENGL_FUNCTION(glBindBufferRange);

			using namespace Game::Graphics::BufferBase;
			using UniformBuffer = Game::Graphics::Classes::UniformBuffer;
			using UniformBuffer_OpenGL = Game::Graphics::Classes::UniformBuffer_OpenGL;

			Game::Graphics::UniformBuffer::Create = WrapperTemplates::Create_OpenGL<UniformBuffer, UniformBuffer_OpenGL>;
			Game::Graphics::UniformBuffer::CreateBulk = WrapperTemplates::CreateBulk_OpenGL<UniformBuffer, UniformBuffer_OpenGL>;

			Game::Graphics::UniformBuffer::Unbind = WrapperTemplates::Unbind_OpenGL<UniformBuffer_OpenGL>;
		}
	}
}