#pragma once

#include <SDL3/SDL_opengl_glext.h>

#include "../../FunctionHeaders/StringHelper.hpp"

#include "ShaderBase.hpp"


namespace Game::Graphics::Classes {

	using FragmentShader = Game::Graphics::Classes::ShaderBase;

	class FragmentShader_OpenGL : public Game::Graphics::Classes::ShaderBase_OpenGL<GL_FRAGMENT_SHADER> {
	public:
		virtual bool SetShaderSource(const char* Source) override {
			/*
			std::string SourceString = Source;
			Uint16 CurrentOutputBinding = 0;
			size_t FragmentOutputIndex, OutputNameIndex;
			while (true) {
				FragmentOutputIndex = SourceString.find("out", FragmentOutputIndex);
				if (FragmentOutputIndex == SIZE_MAX) {
					break;
				}

				if (!StringHelper::IsWhitespace(SourceString[FragmentOutputIndex - 1]) || !StringHelper::IsWhitespace(SourceString[FragmentOutputIndex + 3])) {

				}

				OutputNameIndex = SourceString.find(';', FragmentOutputIndex);
				if (OutputNameIndex == SIZE_MAX) {
					std::cerr << "Missing ';' to close 'out'" << std::endl;
					return false;
				}

				--OutputNameIndex;
				for (; SourceString[OutputNameIndex] == '	' || SourceString[OutputNameIndex] == ' '; --OutputNameIndex) {}
				for (; SourceString[OutputNameIndex] != '	' || SourceString[OutputNameIndex] != ' '; --OutputNameIndex) {}

				//SourceString.replace
			}

			std::cout << SourceString << std::endl;*/

			Game::Graphics::OpenGLFunctions::glShaderSource(this->GLObject, 1, &Source, NULL);
		}

		virtual bool Compile() override {
			Game::Graphics::OpenGLFunctions::glCompileShader(this->GLObject);

			return true;
		}
	};
}

namespace Game::Graphics::FragmentShader {

	Game::Graphics::Classes::FragmentShader*(*Create)();
	void(*CreateBulk)(size_t CreateAmount, Game::Graphics::Classes::FragmentShader* FragmentShaders[]);

	inline void Init() {

		if (Game::Graphics::ActiveAPI == Game::Graphics::GraphicsAPIEnum::OpenGL) {

			using namespace Game::Graphics::ShaderBase;
			using FragmentShader = Game::Graphics::Classes::FragmentShader;
			using FragmentShader_OpenGL = Game::Graphics::Classes::FragmentShader_OpenGL;

			Game::Graphics::FragmentShader::Create = WrapperTemplates::Create_OpenGL<FragmentShader, FragmentShader_OpenGL>;
			Game::Graphics::FragmentShader::CreateBulk = WrapperTemplates::CreateBulk_OpenGL<FragmentShader, FragmentShader_OpenGL>;
		}
	}
}