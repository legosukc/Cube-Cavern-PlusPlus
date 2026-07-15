
local Graphics = Game.Graphics
local Input = Game.Input


local Camera = {
	Position = Vector3.new(0, 0, 0);
	LookDirection = Vector3.new(0, 0, 0);
	UpVector = Vector3.new(0, 1, 0);

	Yaw = -90;
	Pitch = 0;
	Roll = 0;

	FOV = 70;

	CameraHandlerFunc = nil;
}

print("camera look dir: ", Camera.LookDirection)

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

do
	local QuadBuffer = buffer.new(36)

	QuadBuffer:Writef32(0,  -0.5)
	QuadBuffer:Writef32(4,  -0.5)
	QuadBuffer:Writef32(8,   0.0)
	
	QuadBuffer:Writef32(12,  0.5)
	QuadBuffer:Writef32(16, -0.5)
	QuadBuffer:Writef32(20,  0.0)

	QuadBuffer:Writef32(24,  0.0)
	QuadBuffer:Writef32(28,  0.5)
	QuadBuffer:Writef32(32,  0.0)

	Graphics.VertexBuffer.CopyFromBuffer(QuadBuffer)

	QuadBuffer:Writeu8(0, 0)
	QuadBuffer:Writeu8(1, 1)
	QuadBuffer:Writeu8(2, 2)

	Graphics.ElementBuffer.CopyFromBuffer(QuadBuffer, 3)
end

Graphics.VertexArray.SetAttributePointer(0, 3, Graphics.Types.Float, false, 4 * 3)
Graphics.VertexArray.EnableAttribute(0)

Graphics.VertexArray.Unbind()

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
	]]--)

	VertexShader:SetShaderSource([[
		attribute vec3 aPos;
		//uniform mat4 Model;
		uniform mat4 View;
		uniform mat4 Projection;
		
		void main() {
			gl_Position = Projection * View * mat4(1.0) * vec4(aPos, 1.0);
		}
	]])

	local CompileError = VertexShader:Compile()
	if CompileError then
		warn("Failed to compile VertexShader. ", CompileError)
	end

	FragmentShader:SetShaderSource([[
		void main() {
			gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
		}
	]])

	CompileError = FragmentShader:Compile()
	if CompileError then
		warn("Failed to compile FragmentShader. ", CompileError)
	end

	QuadProgram:AttachShader(VertexShader)
	QuadProgram:AttachShader(FragmentShader)

	local ErrorLog = QuadProgram:Link()
	if ErrorLog then
		warn("Failed to link QuadProgram, OpenGL info log:\n", ErrorLog)
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

	Graphics.DrawElements(Graphics.DrawModes.Triangles, 3, Graphics.Types.Uint8)

	--Graphics.VertexArray.Unbind()
end


return Camera