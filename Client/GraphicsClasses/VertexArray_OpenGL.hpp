#pragma once

#include <glad/glad.h>

#include "BaseClassDefinitions.hpp"


//#include "../Statistics.hpp"


namespace Game::Graphics::Classes {

	class VertexArray : public Game::Graphics::Classes::OpenGLClassBase {
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() {}
		virtual bool IsBound() const {
			return false;
		}
	};

	class VertexArray_OpenGL : public VertexArray {
	public:

		virtual ~VertexArray_OpenGL() override {

			if (this->GLObject == 0) {
#ifdef DEBUG_BUILD
				std::cerr << "Attempted to free a unallocated OpenGL VertexArray." << std::endl;
				__debugbreak();
#else
				return;
#endif
			}
			glDeleteVertexArrays(1, &this->GLObject);
		}

		virtual void Bind() override {
			glBindVertexArray(this->GLObject);
		}

		virtual bool IsBound() const override {

			// TODO: FIX THIS!!!!
#warning "FIX THIS!!!!!"
			return true;
			//return this->operator==(Game::Graphics::BoundObjects.VertexArray);
		}
	};
}


namespace {


	static Game::Graphics::Classes::VertexArray _VertexArray_Create_OpenGL() {

		Game::Graphics::Classes::VertexArray NewVertexArray;
		glGenVertexArrays(1, &NewVertexArray.GLObject);

		return NewVertexArray;
	}

	static void _VertexArray_CreateBulk_OpenGL(size_t CreateAmount, Game::Graphics::Classes::VertexArray* VertexArrays) {
		glGenVertexArrays(CreateAmount, reinterpret_cast<GLuint*>(VertexArrays));
	}

	static void _VertexArray_Unbind_OpenGL() {
		glBindVertexArray(0);
	}
}

namespace Game::Graphics::VertexArray {

	Game::Graphics::Classes::VertexArray(*Create)();
	void(*CreateBulk)(size_t CreateAmount, Game::Graphics::Classes::VertexArray* VertexArrays);
	void(*Unbind)();

	inline void Init() {

		if (Game::Graphics::ActiveAPI == Game::Graphics::GraphicsAPIEnum::OpenGL) {

			VertexArray::Create = ::_VertexArray_Create_OpenGL;
			VertexArray::CreateBulk = ::_VertexArray_CreateBulk_OpenGL;

			VertexArray::Unbind = ::_VertexArray_Unbind_OpenGL;
		}
	}
}