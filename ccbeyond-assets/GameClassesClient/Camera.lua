
print("loading camera")

local Graphics = Game.Graphics
local Input = Game.Input


local Camera = {
	Position = Vector3.new(0, 0, 0);
	LookDirection = Vector3.new(0, 0, 0);
	RightVector = Vector3.new(0, 0, 0);
	UpVector = Vector3.new(0, 1, 0);

	ProjectionMatrix = Mat4.new(1);
	ViewMatrix = Mat4.new(1);

	Yaw = -90;
	Pitch = 0;
	Roll = 0;

	FOV = 70;

	CameraHandlerFunc = nil;
}

--local CameraTransformationUniformBuffer = Graphics.UniformBuffer.new()
--Camera.TransformationUniformBuffer = CameraTransformationUniformBuffer

--CameraTransformationUniformBuffer:Bind()
--Graphics.UniformBuffer.AllocateBuffer(Mat4.ByteSize * 2)
--Graphics.UniformBuffer.Unbind()



local VAO = Graphics.VertexArray.new()
local VBO = Graphics.VertexBuffer.new()
local EBO = Graphics.ElementBuffer.new()

VAO:Bind()
VBO:Bind()
EBO:Bind()

local BombfieldIndexCount: number
do
	local Model = ModelLoader.LoadFile("Models/bombfield.obj")

	BombfieldIndexCount = Model.IndexCount

	Graphics.VertexBuffer.CopyFromBuffer(Model.VertexData)
	Graphics.ElementBuffer.CopyFromBuffer(Model.IndexData)

	Graphics.VertexArray.SetAttributePointer(0, 3, Graphics.Types.Float, false, Model.StrideSize, 0)
	Graphics.VertexArray.EnableAttribute(0)

	Graphics.VertexArray.SetAttributePointer(1, 3, Graphics.Types.Float, false, Model.StrideSize, 4 * 3)
	Graphics.VertexArray.EnableAttribute(1)

	Graphics.VertexArray.SetAttributePointer(2, 2, Graphics.Types.Float, false, Model.StrideSize, 4 * 3 * 2)
	Graphics.VertexArray.EnableAttribute(2)

	Graphics.VertexArray.Unbind()
end


local QuadProgram = Graphics.Program.new()
do
	local VertexShader = Graphics.Shader.new(Graphics.ShaderTypes.VertexShader)
	local FragmentShader = Graphics.Shader.new(Graphics.ShaderTypes.FragmentShader)

	--[[
	VertexShader:SetShaderSource([[
		#version 140

		layout(std140) uniform CameraTransformation {
			mat4 Projection, View;
		};
		uniform mat4 Model;
		
		in vec3 aPos;
		
		void main() {
			gl_Position = Projection * View * mat4(1.0) * vec4(aPos, 1.0);
		}
	)]]--

	VertexShader:SetShaderSource([[

		attribute vec3 aPos;
		attribute vec3 aNormal;
		attribute vec2 aTexcoord;

		//uniform mat4 Model;
		uniform mat4 View;
		uniform mat4 Projection;
		
		void main() {
			gl_Position = Projection * View * mat4(1.0) * vec4(aPos, 1.0);
			//gl_Position = vec4(aPos, 1.0);
		}
	]])

	local CompileError = VertexShader:Compile()
	if CompileError then
		print("Failed to compile VertexShader. ", CompileError)
	end

	FragmentShader:SetShaderSource([[
		void main() {
			gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
		}
	]])

	CompileError = FragmentShader:Compile()
	if CompileError then
		print("Failed to compile FragmentShader. ", CompileError)
	end

	QuadProgram:AttachShader(VertexShader)
	QuadProgram:AttachShader(FragmentShader)

	local ErrorLog = QuadProgram:Link()
	if ErrorLog then
		print("Failed to link QuadProgram, OpenGL info log:\n", ErrorLog)
	end

	QuadProgram:DetachAllShaders()

	--QuadProgram:SetUniformBlockBinding(QuadProgram:GetUniformBlockIndex("CameraTransformation"), 0)
end

--local ModelUniformID = QuadProgram:GetUniformIndex("Model")
local ViewUniformID = QuadProgram:GetUniformIndex("View")
local ProjectionUniformID = QuadProgram:GetUniformIndex("Projection")


function Camera:Update()
	
	local ActiveCameraHandler = self.ActiveCameraHandler
	if ActiveCameraHandler then
		ActiveCameraHandler(self)
	end
end

function Camera:Draw()
	
	VAO:Bind()
	--CameraTransformationUniformBuffer:BindToUniformBlockIndex(0)

	QuadProgram:Use()
	
	Graphics.Program.SetUniformMat4(ProjectionUniformID, false, self.ProjectionMatrix)
	Graphics.Program.SetUniformMat4(ViewUniformID, false, self.ViewMatrix)

	Graphics.DrawElements(Graphics.DrawModes.Triangles, BombfieldIndexCount / 3, Graphics.Types.Uint32)

	--Graphics.VertexArray.Unbind()
end


return Camera