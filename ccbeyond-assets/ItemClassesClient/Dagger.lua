local ParentClass = Assets.BaseClassesClient.ItemBase
local class = setmetatable({}, ParentClass)

class.Name = "Dagger"
class.Description = "chuddy forgot to set the description"
class.BookEntry = "chuddy forgot to set the book description"

class.RenderObject = Assets.RenderObjectsClient.rendDagger

class.__index = class

--[[
local DaggerIndexCount: number
do
	local Model = ModelLoader.LoadFile("Models/Dagger.obj")

	DaggerIndexCount = Model.IndexCount

	local ModelVAO = VertexArray.new()
	local ModelVBO = VertexBuffer.new()
	local ModelEBO = ElementBuffer.new()

	ModelVAO:Bind()
	ModelVBO:Bind()
	ModelEBO:Bind()

	Graphics.VertexBuffer.CopyFromBuffer(Model.VertexData)
	Graphics.ElementBuffer.CopyFromBuffer(Model.IndexData)

	Graphics.VertexArray.SetAttributePointer(0, 3, Graphics.Types.Float, false, Model.StrideSize, 0)
	Graphics.VertexArray.EnableAttribute(0)

	Graphics.VertexArray.SetAttributePointer(1, 3, Graphics.Types.Float, false, Model.StrideSize, 4 * 3)
	Graphics.VertexArray.EnableAttribute(1)

	Graphics.VertexArray.SetAttributePointer(2, 2, Graphics.Types.Float, false, Model.StrideSize, 4 * 3 * 2)
	Graphics.VertexArray.EnableAttribute(2)

	Graphics.VertexArray.Unbind()

	local ModelProgram = Program.new()
	do
		local VertexShader = Graphics.Shader.new(Graphics.ShaderTypes.VertexShader)
		VertexShader:LoadShaderSourceFromFile("Shaders/Viewarm.vrt")
		VertexShader:Compile()

		local FragmentShader = Graphics.Shader.new(Graphics.ShaderTypes.FragmentShader)
		FragmentShader:LoadShaderSourceFromFile("Shaders/Viewarm.frg")
		FragmentShader:Compile()

		ModelProgram:AttachShader(VertexShader)
		ModelProgram:AttachShader(FragmentShader)
	
		ModelProgram:Link()

		ModelProgram:DetachAllShaders()
	end

	ModelProgram:Use()

	local ViewportArmTextureUniform = ModelProgram:GetUniformIndex("ArmTexture")
	Graphics.Program.SetUniformSint32(ViewportArmTextureUniform, 0);


	Graphics.Program.SetUniformVec3(ModelProgram:GetUniformIndex("Offset"), Vector3.new(0, 0, 0))
	local ViewportArmProjectionUniform = ModelProgram:GetUniformIndex("Projection")

	class.GraphicsObjects = {
	DaggerVAO = ModelVAO;
	DaggerVBO = ModelVBO;
	DaggerEBO = ModelEBO;

	DaggerProgram = ModelProgram;
}
end]]


function class.new()
	
	local new = ParentClass.new()
	setmetatable(new, class)

	return new
end


function class:UsePrimary()
	print("use primary")
end

function class:UseSecondary()
	print("use secondary")
end


function class:Update()
end

function class:DrawViewmodel(LeftArmOffset, RightArmOffset)
	self.RenderObject:Draw(nil, RightArmOffset + Vector3.new(0, 0, -0.25))
end


function class:OnEquip()
end

function class:OnUnequip()
end


return class