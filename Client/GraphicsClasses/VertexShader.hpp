#pragma once

#include <SDL3/SDL_opengl_glext.h>

#include "ShaderBase.hpp"


namespace Game::Graphics::Classes {

	using VertexShader = Game::Graphics::Classes::ShaderBase;

	class VertexShader_OpenGL : public Game::Graphics::Classes::ShaderBase_OpenGL<GL_VERTEX_SHADER> {};
}

namespace Game::Graphics::VertexShader {

	Game::Graphics::Classes::VertexShader(*Create)();
	void(*CreateBulk)(size_t CreateAmount, Game::Graphics::Classes::VertexShader* VertexShaders);

	inline void Init() {

		if (Game::Graphics::ActiveAPI == Game::Graphics::GraphicsAPIEnum::OpenGL) {

			using namespace Game::Graphics::ShaderBase;
			using VertexShader = Game::Graphics::Classes::VertexShader;
			using VertexShader_OpenGL = Game::Graphics::Classes::VertexShader_OpenGL;

			Game::Graphics::VertexShader::Create = WrapperTemplates::Create_OpenGL<VertexShader, VertexShader_OpenGL>;
			Game::Graphics::VertexShader::CreateBulk = WrapperTemplates::CreateBulk_OpenGL<VertexShader, VertexShader_OpenGL>;
		}
	}
}