#pragma once

#include <SDL3/SDL_opengl_glext.h>

#include "BaseClassDefinitions.hpp"


namespace Game::Graphics::OpenGLFunctions {

	static inline PFNGLCREATESHADERPROC glCreateShader;
	static inline PFNGLDELETESHADERPROC glDeleteShader;

	static inline PFNGLSHADERSOURCEPROC glShaderSource;
	static inline PFNGLGETSHADERSOURCEPROC glGetShaderSource;

	static inline PFNGLCOMPILESHADERPROC glCompileShader;

	static inline PFNGLGETSHADERIVPROC glGetShaderiv;
	static inline PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
}


namespace Game::Graphics::Classes {

	class ShaderBase : public Game::Graphics::Classes::OpenGLClassBase {
	public:
		virtual ~ShaderBase() = default;

		virtual bool SetShaderSource(const char* Source) {
			return false;
		}

		virtual bool Compile() {
			return false;
		}
	};

	template<GLenum ShaderType>
	class ShaderBase_OpenGL : public ShaderBase {
	public:
		virtual ~ShaderBase_OpenGL() override {

			if (this->GLObject == 0) {
#ifdef DEBUG_BUILD
				std::cerr << "Attempted to free a unallocated OpenGL Shader." << std::endl;
				__debugbreak();
#else
				return;
#endif
			}
			Game::Graphics::OpenGLFunctions::glDeleteShader(this->GLObject);
		}

		constexpr static inline GLenum OpenGLShaderType = ShaderType;

		virtual bool SetShaderSource(const char* Source) override {

			Game::Graphics::OpenGLFunctions::glShaderSource(this->GLObject, 1, &Source, NULL);

			return true;
		}

		virtual bool Compile() override {
			Game::Graphics::OpenGLFunctions::glCompileShader(this->GLObject);

			return true;
		}
	};
}

namespace Game::Graphics::ShaderBase {

	namespace WrapperTemplates {

		template<class ShaderClass, class OpenGLShaderClass>
		ShaderClass* Create_OpenGL() {
			static_assert(std::is_base_of_v<Game::Graphics::Classes::ShaderBase, ShaderClass>, "Template class argument 'ShaderClass' isn't derived from 'Game::Graphics::Classes::ShaderBase'.");

			OpenGLShaderClass* NewShader = new OpenGLShaderClass;
			NewShader->GLObject = Game::Graphics::OpenGLFunctions::glCreateShader(OpenGLShaderClass::OpenGLShaderType);

			return NewShader;
		}

		template<class ShaderClass, class OpenGLShaderClass>
		void CreateBulk_OpenGL(size_t CreateAmount, ShaderClass* Shaders[]) {

			OpenGLShaderClass* TempShader;
			size_t i;

			for (i = 0; i < CreateAmount; ++i) {
				TempShader = new OpenGLShaderClass;
				TempShader->GLObject = Game::Graphics::OpenGLFunctions::glCreateShader(OpenGLShaderClass::OpenGLShaderType);
				Shaders[i] = TempShader;
			}
		}
	}


	inline void Init() {

		if (Game::Graphics::ActiveAPI == Game::Graphics::GraphicsAPIEnum::OpenGL) {

			LOAD_OPENGL_FUNCTION(glCreateShader);
			LOAD_OPENGL_FUNCTION(glDeleteShader);

			LOAD_OPENGL_FUNCTION(glShaderSource);
			LOAD_OPENGL_FUNCTION(glGetShaderSource);

			LOAD_OPENGL_FUNCTION(glCompileShader);

			LOAD_OPENGL_FUNCTION(glGetShaderiv);
			LOAD_OPENGL_FUNCTION(glGetShaderInfoLog);
		}
	}
}