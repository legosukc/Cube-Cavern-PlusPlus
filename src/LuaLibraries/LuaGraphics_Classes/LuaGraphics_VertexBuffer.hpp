#pragma once

#include "LuaGraphics_BufferBase.hpp"


namespace Game::Lua::CLibraries::Graphics::Classes {
	struct VertexBuffer : Game::Lua::CLibraries::Graphics::Classes::BufferBase {};
}

namespace Game::Lua::CLibraries::Graphics::VertexBuffer {
	using namespace Game::Lua::CLibraries::Graphics::BufferBase;
}