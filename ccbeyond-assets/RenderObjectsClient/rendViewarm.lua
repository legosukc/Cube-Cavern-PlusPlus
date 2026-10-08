local Graphics = Game.Graphics
local Texture = Graphics.Texture

local ViewportArmVAO = Graphics.VertexArray.new()
local ViewportArmVBO = Graphics.VertexBuffer.new()
local ViewportArmEBO = Graphics.ElementBuffer.new()

ViewportArmVAO:Bind()
ViewportArmVBO:Bind()
ViewportArmEBO:Bind()

local ViewportArmModel = ModelLoader.LoadFile("Models/ViewArm.obj")

Graphics.VertexBuffer.CopyFromBuffer(ViewportArmModel.VertexData)
Graphics.ElementBuffer.CopyFromBuffer(ViewportArmModel.IndexData)

Graphics.VertexArray.EnableAttribute(0)
Graphics.VertexArray.SetAttributePointer(0, 3, Graphics.Types.Float, false, ViewportArmModel.StrideSize, 0)

Graphics.VertexArray.EnableAttribute(1)
Graphics.VertexArray.SetAttributePointer(1, 3, Graphics.Types.Float, false, ViewportArmModel.StrideSize, 4 * 3)

Graphics.VertexArray.EnableAttribute(2)
Graphics.VertexArray.SetAttributePointer(2, 2, Graphics.Types.Float, false, ViewportArmModel.StrideSize, 4 * 3 * 2)

Graphics.VertexArray.Unbind()


local ViewportArmProgram = Graphics.Program.new()
do
	local VertexShader = Graphics.Shader.new(Graphics.ShaderTypes.VertexShader)
	VertexShader:LoadShaderSourceFromFile("Shaders/Viewarm.vrt")
	VertexShader:Compile()

	local FragmentShader = Graphics.Shader.new(Graphics.ShaderTypes.FragmentShader)
	FragmentShader:LoadShaderSourceFromFile("Shaders/Viewarm.frg")
	FragmentShader:Compile()

	ViewportArmProgram:AttachShader(VertexShader)
	ViewportArmProgram:AttachShader(FragmentShader)
	
	ViewportArmProgram:Link()

	ViewportArmProgram:DetachAllShaders()
end

ViewportArmProgram:Use()

local ViewportArmTextureUniform = ViewportArmProgram:GetUniformIndex("ArmTexture")
Graphics.Program.SetUniformSint32(ViewportArmTextureUniform, 0);


local ViewportArmOffsetUniform = ViewportArmProgram:GetUniformIndex("Offset")
local ViewportArmProjectionUniform = ViewportArmProgram:GetUniformIndex("Projection")

print("uniform indexes: ", ViewportArmOffsetUniform, ViewportArmProjectionUniform)

local ViewportArmTexture = Texture.new()
ViewportArmTexture:Bind()

Texture.Load2DImageFromFile("Textures/viewarm_palette.bmp")

Texture.SetFilteringUpscale(Graphics.TextureFiltering.Nearest)
Texture.SetFilteringDownscale(Graphics.TextureFiltering.Nearest)

Texture.SetWrappingHorizontal(Graphics.TextureWrapping.Repeat)
Texture.SetWrappingVertical(Graphics.TextureWrapping.Repeat)


local RenderObject = {
    VAO = ViewportArmVAO;
	VBO = ViewportArmVBO;
	EBO = ViewportArmEBO;

	Program = ViewportArmProgram;
	ArmTextureUniform = ViewportArmTextureUniform;
	ArmOffsetUniform = ViewportArmOffsetUniform;
	ProjectionUniform = ViewportArmProjectionUniform;

    Texture = ViewportArmTexture;

    IndexCount = ViewportArmModel.IndexCount;
}
RenderObject.__index = RenderObject

local SetUniformVec3 = Graphics.Program.SetUniformVec3
local SetUniformMat4 = Graphics.Program.SetUniformMat4
function RenderObject:Draw(NewArmTexture, ArmOffset, ArmRotation, ArmMatrix)
    self.VAO:Bind()
	self.Program:Use();

	(NewArmTexture or self.Texture):Bind()
	Texture.SetActiveTexture(0)
		
	SetUniformVec3(self.ArmOffsetUniform, ArmOffset)
	--Mat4.Rotate(Mat4.Translate(Mat4.new(1), ArmOffset)
	SetUniformMat4(self.ProjectionUniform, false, Game.Camera.ProjectionMatrix)

	Graphics.DrawElements(Graphics.DrawModes.Triangles, self.IndexCount, Graphics.Types.Uint32)
end
    
return RenderObject