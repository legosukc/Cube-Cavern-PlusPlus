#pragma once

#include "BaseClassDefinitions.hpp"

#include <SDL3/SDL_opengl_glext.h>



namespace Game::Graphics::OpenGLFunctions {

	static PFNGLCREATEPROGRAMPROC glCreateProgram;
	static PFNGLDELETEPROGRAMPROC glDeleteProgram;

	static PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
	static PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex;
	static PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding;

	static PFNGLATTACHSHADERPROC glAttachShader;
	static PFNGLGETATTACHEDSHADERSPROC glGetAttachedShaders;
	static PFNGLDETACHSHADERPROC glDetachShader;

	static PFNGLLINKPROGRAMPROC glLinkProgram;
	static PFNGLUSEPROGRAMPROC glUseProgram;

	static PFNGLGETPROGRAMIVPROC glGetProgramiv;

	static PFNGLUNIFORM1IPROC glUniform1i;
	static PFNGLUNIFORM1UIPROC glUniform1ui;
	static PFNGLUNIFORM1FPROC glUniform1f;

	static PFNGLUNIFORM2IVPROC glUniform1iv;
	static PFNGLUNIFORM2UIVPROC glUniform1uiv;
	static PFNGLUNIFORM2FVPROC glUniform1fv;

	static PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv;
	static PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
	static PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
}

namespace Game::Graphics::Classes {

	class Program : public Game::Graphics::Classes::OpenGLClassBase {
	public:
		virtual ~Program() = default;

		virtual GLint GetUniformLocation(const char* UniformName) const {
			return -1;
		}

		virtual bool Link() {
			return false;
		}
		virtual void Use() {}
	};

	class Program_OpenGL : public Program {
	public:
		virtual ~Program_OpenGL() override {
			if (this->GLObject == 0) {
				return;
			}
			Game::Graphics::OpenGLFunctions::glDeleteProgram(this->GLObject);
		}

		virtual GLint GetUniformLocation(const char* UniformName) const override {
			return Game::Graphics::OpenGLFunctions::glGetUniformLocation(this->GLObject, UniformName);
		}

		virtual bool Link() override {
			Game::Graphics::OpenGLFunctions::glLinkProgram(this->GLObject);
			return true;
		}

		virtual void Use() override {
			Game::Graphics::OpenGLFunctions::glUseProgram(this->GLObject);
		}
	};
}


namespace Game::Graphics::Program::Wrappers::OpenGL {

	static Game::Graphics::Classes::Program Create() {

		Game::Graphics::Classes::Program_OpenGL NewProgram;
		NewProgram.GLObject = Game::Graphics::OpenGLFunctions::glCreateProgram();

		return NewProgram;
	}

	static inline void SetUniformSint32(Sint32 UniformIndex, Sint32 Value) {
		Game::Graphics::OpenGLFunctions::glUniform1i(UniformIndex, Value);
	}

	static inline void SetUniformUint32(Sint32 UniformIndex, Uint32 Value) {
		Game::Graphics::OpenGLFunctions::glUniform1ui(UniformIndex, Value);
	}

	static inline void SetUniformBool(Sint32 UniformIndex, bool Value) {
		Game::Graphics::OpenGLFunctions::glUniform1ui(UniformIndex, Value);
	}

	static inline void SetUniformFloat(Sint32 UniformIndex, float Value) {
		Game::Graphics::OpenGLFunctions::glUniform1f(UniformIndex, Value);
	}


	template<class BoolVectorType>
	static inline void SetUniformBvec_(Sint32 UniformIndex, const BoolVectorType BVector) {
		Game::Graphics::OpenGLFunctions::glUniform1iv(UniformIndex, sizeof(BoolVectorType), &BVector);
	}

	template<class VectorType>
	static inline void SetUniform_vec_(Sint32 UniformIndex, const VectorType& Vector) {

		using namespace Game::Graphics::OpenGLFunctions;

		if constexpr (std::is_floating_point_v<typename VectorType::ComponentType>) {
			glUniform1fv(UniformIndex, VectorType::ComponentCount, &Vector.X);

		} else if constexpr (std::is_signed_v<typename VectorType::ComponentType>) {
			glUniform1iv(UniformIndex, VectorType::ComponentCount, &Vector.X);

		} else if constexpr (std::is_unsigned_v<typename VectorType::ComponentType>) {
			glUniform1uiv(UniformIndex, VectorType::ComponentCount, &Vector.X);
		}
	}


	static inline void SetUniformMat2(Sint32 UniformIndex, const Math::Mat2& Mat2) {
		Game::Graphics::OpenGLFunctions::glUniformMatrix2fv(UniformIndex, 1, GL_FALSE, &Mat2.R0.X);
	}

	static inline void SetUniformMat3(Sint32 UniformIndex, const Math::Mat3& Mat3) {
		Game::Graphics::OpenGLFunctions::glUniformMatrix3fv(UniformIndex, 1, GL_FALSE, &Mat3.R0.X);
	}

	static inline void SetUniformMat4(Sint32 UniformIndex, const Math::Mat4& Mat4) {
		Game::Graphics::OpenGLFunctions::glUniformMatrix4fv(UniformIndex, 1, GL_FALSE, &Mat4.R0.X);
	}
}


// TODO: make the SetUniformBvec_ functions use a Bvec_ struct.

namespace Game::Graphics::Program {

	Game::Graphics::Classes::Program(*Create)();

	void(*SetUniformSint32)(Sint32 UniformIndex, Sint32 Value);
	void(*SetUniformUint32)(Sint32 UniformIndex, Uint32 Value);
	void(*SetUniformBool)(Sint32 UniformIndex, bool Value);
	void(*SetUniformFloat)(Sint32 UniformIndex, float Value);

	void(*SetUniformIvec2)(Sint32 UniformIndex, const Math::IVector2& Ivec2);
	void(*SetUniformUvec2)(Sint32 UniformIndex, const Math::UVector2& Uvec2);
	void(*SetUniformBvec2)(Sint32 UniformIndex, const Math::IVector2& Bvec2);
	void(*SetUniformFvec2)(Sint32 UniformIndex, const Math::Vector2& Fvec2);

	void(*SetUniformIvec3)(Sint32 UniformIndex, const Math::IVector3& Ivec3);
	void(*SetUniformUvec3)(Sint32 UniformIndex, const Math::UVector3& Uvec3);
	void(*SetUniformBvec3)(Sint32 UniformIndex, const Math::IVector3& Bvec3);
	void(*SetUniformFvec3)(Sint32 UniformIndex, const Math::Vector3& Fvec3);

	void(*SetUniformIvec4)(Sint32 UniformIndex, const Math::IVector4& Ivec4);
	void(*SetUniformUvec4)(Sint32 UniformIndex, const Math::UVector4& Uvec4);
	void(*SetUniformBvec4)(Sint32 UniformIndex, const Math::IVector4& Bvec4);
	void(*SetUniformFvec4)(Sint32 UniformIndex, const Math::Vector4& Fvec4);

	void(*SetUniformMat2)(Sint32 UniformIndex, const Math::Mat2& Mat2);
	void(*SetUniformMat3)(Sint32 UniformIndex, const Math::Mat3& Mat3);
	void(*SetUniformMat4)(Sint32 UniformIndex, const Math::Mat4& Mat4);

	inline void Init() {

		if (Game::Graphics::ActiveAPI == Game::Graphics::GraphicsAPIEnum::OpenGL) {
			
			LOAD_OPENGL_FUNCTION(glCreateProgram);
			LOAD_OPENGL_FUNCTION(glDeleteProgram);

			LOAD_OPENGL_FUNCTION(glGetUniformLocation);
			LOAD_OPENGL_FUNCTION(glGetUniformBlockIndex);

			LOAD_OPENGL_FUNCTION(glUniformBlockBinding);

			LOAD_OPENGL_FUNCTION(glAttachShader);
			LOAD_OPENGL_FUNCTION(glGetAttachedShaders);
			LOAD_OPENGL_FUNCTION(glDetachShader);

			LOAD_OPENGL_FUNCTION(glLinkProgram);
			LOAD_OPENGL_FUNCTION(glUseProgram);

			LOAD_OPENGL_FUNCTION(glGetProgramiv);

			LOAD_OPENGL_FUNCTION(glUniform1i);
			LOAD_OPENGL_FUNCTION(glUniform1ui);
			LOAD_OPENGL_FUNCTION(glUniform1f);

			LOAD_OPENGL_FUNCTION(glUniformMatrix2fv);
			LOAD_OPENGL_FUNCTION(glUniformMatrix3fv);
			LOAD_OPENGL_FUNCTION(glUniformMatrix4fv);

			Program::Create = Game::Graphics::Program::Wrappers::OpenGL::Create;

			Program::SetUniformSint32 = Game::Graphics::Program::Wrappers::OpenGL::SetUniformSint32;
			Program::SetUniformUint32 = Game::Graphics::Program::Wrappers::OpenGL::SetUniformUint32;
			Program::SetUniformBool = Game::Graphics::Program::Wrappers::OpenGL::SetUniformBool;
			Program::SetUniformFloat = Game::Graphics::Program::Wrappers::OpenGL::SetUniformFloat;

			Program::SetUniformIvec2 = Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec_<Math::IVector2>;
			Program::SetUniformUvec2 = Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec_<Math::UVector2>;
			Program::SetUniformBvec2 = Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec_<Math::IVector2>;
			Program::SetUniformFvec2 = Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec_<Math::Vector2>;

			Program::SetUniformIvec3 = Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec_<Math::IVector3>;
			Program::SetUniformUvec3 = Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec_<Math::UVector3>;
			Program::SetUniformBvec3 = Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec_<Math::IVector3>;
			Program::SetUniformFvec3 = Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec_<Math::Vector3>;

			Program::SetUniformIvec4 = Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec_<Math::IVector4>;
			Program::SetUniformUvec4 = Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec_<Math::UVector4>;
			Program::SetUniformBvec4 = Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec_<Math::IVector4>;
			Program::SetUniformFvec4 = Game::Graphics::Program::Wrappers::OpenGL::SetUniform_vec_<Math::Vector4>;

			Program::SetUniformMat2 = Game::Graphics::Program::Wrappers::OpenGL::SetUniformMat2;
			Program::SetUniformMat3 = Game::Graphics::Program::Wrappers::OpenGL::SetUniformMat3;
			Program::SetUniformMat4 = Game::Graphics::Program::Wrappers::OpenGL::SetUniformMat4;
		}
	}
}