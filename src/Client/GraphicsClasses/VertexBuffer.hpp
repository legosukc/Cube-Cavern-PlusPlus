#pragma once

#include <SDL3/SDL_opengl_glext.h>

#include "BaseClassDefinitions.hpp"


namespace Game::Graphics::Classes {

	using VertexBuffer = Game::Graphics::Classes::BufferBase;

	class VertexBuffer_OpenGL : public BufferBase_OpenGL<GL_ARRAY_BUFFER> {
	public:

		virtual bool IsBound() const override {
			// TODO: FIX THIS!!!!
			return true;
		}
	};
}


namespace Game::Graphics::VertexBuffer {

	Game::Graphics::Classes::VertexBuffer*(*Create)();
	void(*CreateBulk)(size_t CreateAmount, Game::Graphics::Classes::VertexBuffer* VertexBuffers[]);
	void(*Unbind)();

	inline void Init() {

		if (Game::Graphics::ActiveAPI == Game::Graphics::GraphicsAPIEnum::OpenGL) {

			using namespace Game::Graphics::BufferBase;
			using VertexBuffer = Game::Graphics::Classes::VertexBuffer;
			using VertexBuffer_OpenGL = Game::Graphics::Classes::VertexBuffer_OpenGL;

			Game::Graphics::VertexBuffer::Create = WrapperTemplates::Create_OpenGL<VertexBuffer, VertexBuffer_OpenGL>;
			Game::Graphics::VertexBuffer::CreateBulk = WrapperTemplates::CreateBulk_OpenGL<VertexBuffer, VertexBuffer_OpenGL>;

			Game::Graphics::VertexBuffer::Unbind = WrapperTemplates::Unbind_OpenGL<VertexBuffer_OpenGL>;
		}
	}
}