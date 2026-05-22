#pragma once

#include <glad/glad.h>


#include "../Statistics.hpp"


namespace Game::Graphics {
	inline void Init();

	enum class GraphicsAPIEnum : Uint8 {
		OpenGL, Vulkan,
		Direct3D11, Direct3D12,
		Metal
	};

	GraphicsAPIEnum ActiveAPI;

	void(*SetClearColor)(float R, float G, float B, float A);
	void(*ClearBitfields)(unsigned int Bitfields);


	inline void Update();
	void Draw();
}

namespace {

	struct _Enums_BindTargets {
		constexpr _Enums_BindTargets() = default;

		using IntType = unsigned int;

		IntType VertexBuffer{}, ElementBuffer{}, UniformBuffer{};

	private:
		constexpr _Enums_BindTargets(IntType _VertexBuffer, IntType _ElementBuffer, IntType _UniformBuffer)
			: VertexBuffer(_VertexBuffer)
			, ElementBuffer(_ElementBuffer)
			, UniformBuffer(_UniformBuffer) {}

		friend inline void Game::Graphics::Init();
	};


	struct _Enums_AllocationTypes {
		constexpr _Enums_AllocationTypes() = default;

		using IntType = unsigned int;

		IntType Streaming{}, Drawing{}, Reading{};

	private:
		constexpr _Enums_AllocationTypes(IntType _Streaming, IntType _Drawing, IntType _Reading)
			: Streaming(_Streaming)
			, Drawing(_Drawing)
			, Reading(_Reading) {}

		friend inline void Game::Graphics::Init();
	};


	struct _Enums_BufferBitfields {
		constexpr _Enums_BufferBitfields() = default;

		using IntType = unsigned int;

		IntType ColorBit{}, DepthBit{}, StencilBit{};

	private:
		constexpr _Enums_BufferBitfields(IntType _ColorBit, IntType _DepthBit, IntType _StencilBit)
			: ColorBit(_ColorBit)
			, DepthBit(_DepthBit)
			, StencilBit(_StencilBit) {}

		friend inline void Game::Graphics::Init();
	};



	struct _Namespace_Buffer {
		virtual void CopyToBuffer(_Enums_BindTargets::IntType BindTarget, size_t Size, const void* Data, _Enums_AllocationTypes::IntType AllocateFor) {}
		virtual void CopyToBufferPointer(_Enums_BindTargets::IntType BindTarget, int Offset, size_t Size, const void* Data) {}
		virtual void AllocateBuffer(_Enums_BindTargets::IntType BindTarget, size_t Size, _Enums_AllocationTypes::IntType AllocateFor) {}
	};
}

namespace Game::Graphics {
	::_Enums_BindTargets BindTargets;
	::_Enums_AllocationTypes AllocationTypes;
	::_Enums_BufferBitfields BufferBitfields;

	::_Namespace_Buffer Buffer;
}


namespace {


	struct _Namespace_Buffer_OpenGL : ::_Namespace_Buffer {

		virtual void CopyToBuffer(_Enums_BindTargets::IntType BindTarget, size_t Size, const void* Data, _Enums_AllocationTypes::IntType AllocateFor) override {
			glBufferData(BindTarget, Size, Data, AllocateFor);
		}

		virtual void CopyToBufferPointer(_Enums_BindTargets::IntType BindTarget, int Offset, size_t Size, const void* Data) override {
			glBufferSubData(BindTarget, Offset, Size, Data);
		}

		virtual void AllocateBuffer(_Enums_BindTargets::IntType BindTarget, size_t Size, _Enums_AllocationTypes::IntType AllocateFor) override {
			glBufferData(BindTarget, Size, NULL, AllocateFor);
		}
	};


	namespace _BasicFunctions {

		static inline void _SetClearColor_OpenGL(float R, float G, float B, float A) {
			glClearColor(R, G, B, A);
		}

		static inline void _ClearBitfields_OpenGL(unsigned int Bitfields) {
			glClear(Bitfields);
		}
	}
}


#include "GraphicsClasses/VertexArray_OpenGL.hpp"

/*
namespace Game::Graphics::Classes {

	struct OpenGLClassBase {
		GLuint GLObject = 0;

		constexpr bool operator==(const OpenGLClassBase& B) const {
			return this->GLObject == B.GLObject;
		}
	};

	template<GLenum BufferType>
	class BufferBase : public Game::Graphics::Classes::OpenGLClassBase {
	public:
		inline ~BufferBase() {

#ifdef DEBUG_BUILD
			if (this->GLObject == 0) {
				std::cerr << "Attempted to free a unallocated OpenGL Buffer." << std::endl;
				__debugbreak();
			}
#endif
			glDeleteBuffers(1, &this->GLObject);
		}

		inline void Create() {
			glGenBuffers(1, &this->GLObject);
		}

		inline void Bind(GLenum BindTarget = BufferType) {
			glBindBuffer(BindTarget, this->GLObject);
		}

		inline bool IsBound(GLenum BindTarget = BufferType) const {
			return;
		}
	};


	class VertexArray;
	class VertexBuffer : public BufferBase<GL_ARRAY_BUFFER> {};
	class ElementBuffer : public BufferBase<GL_ELEMENT_ARRAY_BUFFER> {};
	class UniformBuffer;

	class Program;
	class Shader;

	class Texture;
}


class Game::Graphics::Classes::VertexArray : public Game::Graphics::Classes::OpenGLClassBase {
public:

	inline ~VertexArray() {

#ifdef DEBUG_BUILD
		if (this->GLObject == 0) {
			std::cerr << "Attempted to free a unallocated OpenGL VertexArray." << std::endl;
			__debugbreak();
		}
#endif
		glDeleteVertexArrays(1, &this->GLObject);
	}

	inline void Create() {
		glGenVertexArrays(1, &this->GLObject);
	}

	inline void Bind() {
		glBindVertexArray(this->GLObject);
	}

	constexpr bool IsBound() const {
		return this->operator==(Game::Graphics::BoundObjects.VertexArray);
	}
};


class Game::Graphics::Classes::UniformBuffer : public Game::Graphics::Classes::BufferBase<GL_UNIFORM_BUFFER> {
public:
};




class Game::Graphics::Classes::Program : public Game::Graphics::Classes::OpenGLClassBase {
public:
	inline ~Program() {
		glDeleteProgram(this->GLObject);
	}

	inline void Create() {
		this->GLObject = glCreateProgram();
	}
};


class Game::Graphics::Classes::Shader {
public:
	inline ~Shader() {
		glDeleteShader(this->GLObject);
	}

	inline void Create(GLenum ShaderType) {
		this->GLObject = glCreateShader(ShaderType);
	}

	GLint GLObject;
};




class Game::Graphics::Classes::Texture : public Game::Graphics::Classes::OpenGLClassBase {
public:
	inline ~Texture() {
		glDeleteTextures(1, &this->GLObject);
	}

	inline void Create() {
		glGenTextures(1, &this->GLObject);
	}
};*/

namespace Game::Graphics {

	struct BoundObjectsStruct {
		Classes::VertexArray VertexArray;
		Classes::VertexBuffer VertexBuffer;
		Classes::ElementBuffer ElementBuffer;
		Classes::UniformBuffer UniformBuffer;

		Classes::Program Program;
		Classes::Texture Texture;
	} BoundObjects;
}


void Game::Graphics::Init() {

	Game::Graphics::ActiveAPI = Game::Graphics::GraphicsAPIEnum::OpenGL;

	switch (Game::Graphics::ActiveAPI) {

	case Game::Graphics::GraphicsAPIEnum::OpenGL:


		Game::Graphics::SetClearColor = ::_BasicFunctions::_SetClearColor_OpenGL;
		Game::Graphics::ClearBitfields = ::_BasicFunctions::_ClearBitfields_OpenGL;

		Graphics::BindTargets = ::_Enums_BindTargets(GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, GL_UNIFORM_BUFFER);

		Graphics::AllocationTypes = ::_Enums_AllocationTypes(GL_STREAM_DRAW, GL_STATIC_DRAW, GL_STATIC_READ);
		Graphics::BufferBitfields = ::_Enums_BufferBitfields(GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT);

		Graphics::Buffer = ::_Namespace_Buffer_OpenGL();


		break;

	case Game::Graphics::GraphicsAPIEnum::Vulkan:
	case Game::Graphics::GraphicsAPIEnum::Direct3D11:
	case Game::Graphics::GraphicsAPIEnum::Direct3D12:
	case Game::Graphics::GraphicsAPIEnum::Metal:
		break;
	};

	
	Game::Graphics::VertexArray::Init();
}


void Game::Graphics::Update() {

}

void Game::Graphics::Draw() {

}