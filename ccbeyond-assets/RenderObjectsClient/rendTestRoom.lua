local RenderObject = {}
RenderObject.__index = RenderObject

local Graphics = Game.Graphics

local VertexArray = Graphics.VertexArray

do
local VAO = VertexArray.new()
local VBO = Graphics.VertexBuffer.new()
local EBO = Graphics.ElementBuffer.new()

VAO:Bind()
VBO:Bind()
EBO:Bind()

local Model = ModelLoader.LoadFile("Models/Test.obj")

Graphics.VertexBuffer.CopyFromBuffer(Model.VertexData)
Graphics.ElementBuffer.CopyFromBuffer(Model.IndexData)

VertexArray.EnableAttribute(0)
VertexArray.SetAttributePointer(0, 3, Graphics.Types.Float, false, Model.StrideSize, 0)

VertexArray.EnableAttribute(1)
VertexArray.SetAttributePointer(1, 3, Graphics.Types.Float, false, Model.StrideSize, 4 * 3)

VertexArray.EnableAttribute(2)
VertexArray.SetAttributePointer(2, 2, Graphics.Types.Float, false, Model.StrideSize, 4 * 3 * 2)

VertexArray.Unbind()


local ShaderProgram = Graphics.Program.new()
do
	local VertexShader = Graphics.Shader.new(Graphics.ShaderTypes.VertexShader)
	VertexShader:LoadShaderSourceFromFile("Shaders/test.vrt")
	VertexShader:Compile()

	local FragmentShader = Graphics.Shader.new(Graphics.ShaderTypes.FragmentShader)
	FragmentShader:LoadShaderSourceFromFile("Shaders/test.frg")
	FragmentShader:Compile()

	ShaderProgram:AttachShader(VertexShader)
	ShaderProgram:AttachShader(FragmentShader)
	
	ShaderProgram:Link()

	ShaderProgram:DetachAllShaders()
end

    RenderObject.VAO = VAO
	RenderObject.VBO = VBO
	RenderObject.EBO = EBO

	RenderObject.Program = ShaderProgram
	RenderObject.ProjectionUniform = ShaderProgram:GetUniformIndex("Projection")
    RenderObject.ViewUniform = ShaderProgram:GetUniformIndex("View")

    RenderObject.IndexCount = Model.IndexCount
end

local SetUniformMat4 = Graphics.Program.SetUniformMat4
function RenderObject:Draw()

	--Graphics.EnableDepthTest()

    self.VAO:Bind()
	self.Program:Use()
		
	SetUniformMat4(self.ProjectionUniform, false, Game.Camera.ProjectionMatrix)
    SetUniformMat4(self.ViewUniform, false, Game.Camera.ViewMatrix)

	Graphics.DrawElements(Graphics.DrawModes.Triangles, self.IndexCount, Graphics.Types.Uint32)
end
    
return RenderObject