#ifndef LUAGRAPHICS_DEFAULTPROGRAMS
#define LUAGRAPHICS_DEFAULTPROGRAMS

#include "../../../include/VM/lua.h"
#include "../../../include/VM/lualib.h"

#include "../../../include/Compiler/luacode.h"

#include "../../FunctionHeaders/LuaHelper.hpp"

#include "../LuaGraphics_Classes/LuaGraphics_Program.hpp"
#include "../LuaGraphics_Classes/LuaGraphics_Shader.hpp"

namespace Game::Lua::CLibraries::Graphics::DefaultPrograms {
    inline void Init(lua_State* State,
                     LuaHelper::StackTableReference& GraphicsTable);
}

static const char* script = R"C(
local Graphics = Game.Graphics
return function(VertexShaderSource, FragmentShaderSource)
    local Program = Graphics.Program.new()

    local Shader = Graphics.Shader.new(Graphics.ShaderTypes.VertexShader)
    Shader:SetShaderSource(VertexShaderSource)
    local CompileError = Shader:Compile()
    if CompileError then
        error(CompileError)
    end
    Program:AttachShader(Shader)

    Shader = Graphics.Shader.new(Graphics.ShaderTypes.FragmentShader)
    Shader:SetShaderSource(FragmentShaderSource)
    local CompileError = Shader:Compile()
    if CompileError then
        error(CompileError)
    end
    Program:AttachShader(Shader)

    Program:Link()
end
)C";

void Game::Lua::CLibraries::Graphics::DefaultPrograms::Init(lua_State* State, LuaHelper::StackTableReference& GraphicsTable) {
    LuaHelper::StackTableReference DefaultPrograms(State, 0, 4);

    CLibraries::Graphics::Program::__new(State);


            CLibraries::Graphics::Classes::Program::AttachShader(State);
}

#endif