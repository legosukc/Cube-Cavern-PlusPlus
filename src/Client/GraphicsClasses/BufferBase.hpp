#pragma once

#include "BaseClassDefinitions.hpp"


namespace Game::Graphics::OpenGLFunctions {

	static PFNGLGENBUFFERSPROC glGenBuffers;
	static PFNGLDELETEBUFFERSPROC glDeleteBuffers;

	static PFNGLBUFFERDATAPROC glBufferData;
	static PFNGLBUFFERSUBDATAPROC glBufferSubData;

	static PFNGLBINDBUFFERPROC glBindBuffer;
}

namespace Game::Graphics::Classes {

	class BufferBase : public Game::Graphics::Classes::OpenGLClassBase {
	public:
		virtual ~BufferBase() = default;

		virtual void Bind() {}
		virtual bool IsBound() const {
			return false;
		}
	};

	template<GLenum BufferTarget>
	class BufferBase_OpenGL : virtual public BufferBase {
	public:
		virtual ~BufferBase_OpenGL() override {

			if (this->GLObject == 0) {
#ifdef DEBUG_BUILD
				std::cerr << "Attempted to free an unallocated OpenGL buffer." << std::endl;
				__debugbreak();
#else
				return;
#endif
			}
			Game::Graphics::OpenGLFunctions::glDeleteBuffers(1, &this->GLObject);
		}

		virtual void Bind() override {
			Game::Graphics::OpenGLFunctions::glBindBuffer(BufferTarget, this->GLObject);
		}

		constexpr static inline GLenum OpenGLBufferEnum = BufferTarget;
	};
}


namespace Game::Graphics::BufferBase {

	namespace WrapperTemplates {

		template<class BufferClass, class BufferOpenGLClass>
		BufferClass* Create_OpenGL() {

			BufferOpenGLClass* NewBuffer = new BufferOpenGLClass;
			Game::Graphics::OpenGLFunctions::glGenBuffers(1, &NewBuffer->GLObject);

			return NewBuffer;
		}

		template<class BufferClass, class BufferOpenGLClass>
		void CreateBulk_OpenGL(size_t CreateAmount, BufferClass* Buffers[]) {

			for (size_t i = 0; i < CreateAmount; ++i) {

				Buffers[i] = new BufferOpenGLClass();
				Game::Graphics::OpenGLFunctions::glGenBuffers(1, &Buffers[i]->GLObject);
			}
		}

		template<class BufferOpenGLClass>
		inline void Unbind_OpenGL() {
			Game::Graphics::OpenGLFunctions::glBindBuffer(BufferOpenGLClass::OpenGLBufferEnum, 0);
		}
	}

	inline void Init() {

		if (Game::Graphics::ActiveAPI == Game::Graphics::GraphicsAPIEnum::OpenGL) {

			LOAD_OPENGL_FUNCTION(glGenBuffers);
			LOAD_OPENGL_FUNCTION(glDeleteBuffers);

			LOAD_OPENGL_FUNCTION(glBindBuffer);

			LOAD_OPENGL_FUNCTION(glBufferData);
			LOAD_OPENGL_FUNCTION(glBufferSubData);
		}
	}
}