#pragma once

#include <SDL3/SDL_opengl_glext.h>

#include "BaseClassDefinitions.hpp"


namespace Game::Graphics::Classes {

	using IndexBuffer = Game::Graphics::Classes::BufferBase;

	class IndexBuffer_OpenGL : public BufferBase_OpenGL<GL_ELEMENT_ARRAY_BUFFER> {
	public:

		virtual bool IsBound() const override {
			// TODO: FIX THIS!!!!
			return true;
		}
	};
}


namespace Game::Graphics::IndexBuffer {

	Game::Graphics::Classes::IndexBuffer*(*Create)();
	void(*CreateBulk)(size_t CreateAmount, Game::Graphics::Classes::IndexBuffer* IndexBuffers[]);
	void(*Unbind)();

	inline void Init() {

		if (Game::Graphics::ActiveAPI == Game::Graphics::GraphicsAPIEnum::OpenGL) {

			using namespace Game::Graphics::BufferBase;
			using IndexBuffer = Game::Graphics::Classes::IndexBuffer;
			using IndexBuffer_OpenGL = Game::Graphics::Classes::IndexBuffer_OpenGL;

			Game::Graphics::IndexBuffer::Create = WrapperTemplates::Create_OpenGL<IndexBuffer, IndexBuffer_OpenGL>;
			Game::Graphics::IndexBuffer::CreateBulk = WrapperTemplates::CreateBulk_OpenGL<IndexBuffer, IndexBuffer_OpenGL>;

			Game::Graphics::IndexBuffer::Unbind = WrapperTemplates::Unbind_OpenGL<IndexBuffer_OpenGL>;
		}
	}
}