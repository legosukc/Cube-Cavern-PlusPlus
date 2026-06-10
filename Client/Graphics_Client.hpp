#pragma once

#include "../define.h"

#include <SDL3/SDL_video.h>

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#include <SDL3/SDL_opengles2_gl2ext.h>

#include "../Statistics.hpp"


namespace Game::Graphics {
	inline void Init();

	enum class WindowMode : Uint8 {
		Windowed, Borderless, Fullscreen
	};

	Uint8 AnisotropicFilteringLevel = 1;

	namespace OpenGL {
		int MajorVersion;
		int MinorVersion;
		int Profile;
	}

	enum class GraphicsAPIEnum : Uint8 {
		OpenGL, Vulkan,
		Direct3D11, Direct3D12,
		Metal
	};

	GraphicsAPIEnum ActiveAPI;

	namespace {
		static void(*_SetClearColor)(float R, float G, float B, float A);
		static void(*_ClearBitfields)(unsigned int Bitfields);
	}

	SDL_FORCE_INLINE void SetClearColor(float R, float G, float B, float A) {
		_SetClearColor(R, G, B, A);
	}

	SDL_FORCE_INLINE void ClearBitfields(unsigned int Bitfields) {
		_ClearBitfields(Bitfields);
	}


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

		friend inline void ::Game::Graphics::Init();
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

		friend inline void ::Game::Graphics::Init();
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

		friend inline void ::Game::Graphics::Init();
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
}


namespace Game::Graphics::OpenGLFunctions {
	static PFNGLCLEARCOLORPROC glClearColor;
	static PFNGLCLEARPROC glClear;

	static PFNGLGETINTEGERVPROC glGetIntegerv;
	static PFNGLISENABLEDPROC glIsEnabled;

	static PFNGLDRAWARRAYSPROC glDrawArrays;
	static PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced;

	static PFNGLDRAWELEMENTSPROC glDrawElements;
	static PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced;
}

namespace {

	namespace _BasicFunctions {

		static inline void _SetClearColor_OpenGL(float R, float G, float B, float A) {
			Game::Graphics::OpenGLFunctions::glClearColor(R, G, B, A);
		}

		static inline void _ClearBitfields_OpenGL(unsigned int Bitfields) {
			Game::Graphics::OpenGLFunctions::glClear(Bitfields);
		}
	}
}


#define LOAD_OPENGL_FUNCTION(FunctionName)(Game::Graphics::OpenGLFunctions:: FunctionName = (decltype(Game::Graphics::OpenGLFunctions:: FunctionName))SDL_GL_GetProcAddress(#FunctionName))

#include "GraphicsClasses/BaseClassDefinitions.hpp"
#include "GraphicsClasses/BufferBase.hpp"

#include "GraphicsClasses/VertexArray.hpp"
#include "GraphicsClasses/VertexBuffer.hpp"
#include "GraphicsClasses/IndexBuffer.hpp"

#include "GraphicsClasses/UniformBuffer.hpp"

#include "GraphicsClasses/ShaderBase.hpp"
#include "GraphicsClasses/VertexShader.hpp"
#include "GraphicsClasses/FragmentShader.hpp"

#include "GraphicsClasses/Program.hpp"

#include "GraphicsClasses/Texture.hpp"
#include "GraphicsClasses/Framebuffer.hpp"


namespace Game::Graphics {

	struct BoundObjectsStruct {
		Classes::VertexArray* VertexArray;
		Classes::VertexBuffer* VertexBuffer;
		Classes::IndexBuffer* IndexBuffer;
		Classes::UniformBuffer* UniformBuffer;

		Classes::Program* Program;
		Classes::Texture* Texture;
	} BoundObjects;

	struct {
		Classes::Texture* Position, *Normal, *Albedo, *Specular;
	} GBuffer;
}


void Game::Graphics::Init() {

	Graphics::ActiveAPI = Game::Graphics::GraphicsAPIEnum::OpenGL;

	switch (Graphics::ActiveAPI) {
	case Graphics::GraphicsAPIEnum::OpenGL:
		
		SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &Graphics::OpenGL::MajorVersion);
		SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &Graphics::OpenGL::MinorVersion);
		SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &Graphics::OpenGL::Profile);

		if (Graphics::OpenGL::MajorVersion < 3) {

			static const char* Extensions[] = {
				"GL_EXT_draw_buffers"
			};
			
			//PFNGLGENVERTEXARRAYSOESPROC
			for (const char* Extension : Extensions) {
				if (!SDL_GL_ExtensionSupported(Extension)) {
					std::cerr << "" << std::endl;
				}
			}
		}

		LOAD_OPENGL_FUNCTION(glClear);
		LOAD_OPENGL_FUNCTION(glClearColor);

		LOAD_OPENGL_FUNCTION(glDrawArrays);
		LOAD_OPENGL_FUNCTION(glDrawArraysInstanced);

		LOAD_OPENGL_FUNCTION(glDrawElements);
		LOAD_OPENGL_FUNCTION(glDrawElementsInstanced);

		LOAD_OPENGL_FUNCTION(glGetIntegerv);
		LOAD_OPENGL_FUNCTION(glIsEnabled);

		::Game::Graphics::_SetClearColor = ::_BasicFunctions::_SetClearColor_OpenGL;
		::Game::Graphics::_ClearBitfields = ::_BasicFunctions::_ClearBitfields_OpenGL;

		::Game::Graphics::BindTargets = ::_Enums_BindTargets(GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, GL_UNIFORM_BUFFER);

		::Game::Graphics::AllocationTypes = ::_Enums_AllocationTypes(GL_STREAM_DRAW, GL_STATIC_DRAW, GL_STATIC_READ);
		::Game::Graphics::BufferBitfields = ::_Enums_BufferBitfields(GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT);

		break;

	case GraphicsAPIEnum::Vulkan:
		break;
	case GraphicsAPIEnum::Direct3D11:
		break;
	case GraphicsAPIEnum::Direct3D12:
		break;
	case GraphicsAPIEnum::Metal:
		break;
	};

	
	Graphics::BufferBase::Init();

	Graphics::VertexArray::Init();
	Graphics::VertexBuffer::Init();
	Graphics::IndexBuffer::Init();

	Graphics::UniformBuffer::Init();

	Graphics::ShaderBase::Init();
	Graphics::VertexShader::Init();
	Graphics::FragmentShader::Init();

	Graphics::Program::Init();

	Graphics::Texture::Init();

	using Game::Graphics::GBuffer;
	Graphics::Texture::CreateBulk(4, reinterpret_cast<Classes::Texture**>(&GBuffer));

	const Math::IVector2 WindowSize = Game::Window.GetSize();

	for (int i = 0; i < 4; ++i) {
		reinterpret_cast<Classes::Texture**>(&GBuffer)[i]->Bind();
		Graphics::Texture::UploadPixelData(WindowSize.X, WindowSize.Y, GL_RGB, GL_FLOAT, NULL);
		Graphics::Texture::AttachToFramebuffer(GL_COLOR_ATTACHMENT0 + i, reinterpret_cast<Classes::Texture**>(&GBuffer)[i]);
	}
}


void Game::Graphics::Update() {

}

void Game::Graphics::Draw() {

}