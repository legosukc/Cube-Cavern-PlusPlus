local Graphics = Game.Graphics
local Texture = Graphics.Texture

	local ViewportArmVAO = Graphics.VertexArray.new()
	local ViewportArmVBO = Graphics.VertexBuffer.new()
	local ViewportArmEBO = Graphics.ElementBuffer.new()

	ViewportArmVAO:Bind()

	local ViewportArmModel = ModelLoader.LoadFile("Models/ViewArm.obj")

    ViewportArmVBO:Bind()
	Graphics.VertexBuffer.CopyFromBuffer(ViewportArmModel.VertexData)

    ViewportArmEBO:Bind()
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

		local CompileError = VertexShader:Compile()
        assert(CompileError, "Failed to compile VertexShader. "..CompileError)

		local FragmentShader = Graphics.Shader.new(Graphics.ShaderTypes.FragmentShader)
		FragmentShader:LoadShaderSourceFromFile("Shaders/Viewarm.frg")

		CompileError = FragmentShader:Compile()
        assert(CompileError, "Failed to compile FragmentShader. "..CompileError)

		ViewportArmProgram:AttachShader(VertexShader)
		ViewportArmProgram:AttachShader(FragmentShader)
	
		local ErrorLog = ViewportArmProgram:Link()
        assert(ErrorLog, "Failed to link ViewportArmProgram, OpenGL info log:\n"..ErrorLog)

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

	    ArmTexture = ViewportArmTexture;

        IndexCount = ViewportArmModel.IndexCount;
    }

local SetUniformVec3 = Graphics.Program.SetUniformVec3
local SetUniformMat4 = Graphics.Program.SetUniformMat4
function RenderObject:Draw(Texture, ArmOffset)
    self.VAO:Bind()
			self.Program:Use();

			(Texture or self.ArmTexture):Bind()
			Texture.SetActiveTexture(0)
		
			SetUniformVec3(self.ArmOffsetUniform, ArmOffset)
			SetUniformMat4(self.ProjectionUniform, false, Game.Camera.ProjectionMatrix)

			Graphics.DrawElements(Graphics.DrawModes.Triangles, self.IndexCount, Graphics.Types.Uint32)
end
    
return RenderObject