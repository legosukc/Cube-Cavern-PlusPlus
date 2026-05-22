
local Graphics = Game.Graphics
local Input = Game.Input


local Camera = {
	Position = Vector3.new();
	LookDirection = Vector3.new(0, 0, 0);
	UpVector = Vector3.new(0, 1, 0);

	FOV = 70;
}

print("camera look dir: ", Camera.LookDirection)

local CameraTransformationUniformBuffer = Graphics.UniformBuffer.new()
Camera.TransformationUniformBuffer = CameraTransformationUniformBuffer

CameraTransformationUniformBuffer:Bind()
Graphics.UniformBuffer.AllocateBuffer(Mat4.ByteSize * 2)
--Graphics.UniformBuffer.Unbind()



local VAO = Game.Graphics.VertexArray.new()
local VBO = Game.Graphics.VertexBuffer.new()
local EBO = Game.Graphics.ElementBuffer.new()

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

Graphics.VertexArray.EnableAttribute(0)
Graphics.VertexArray.SetAttributePointer(0, 3, Graphics.Types.Float, false, 4 * 3)

--Graphics.VertexArray.Unbind()
--Graphics.VertexBuffer.Unbind()
--Graphics.ElementBuffer.Unbind()


local QuadProgram = Graphics.Program.new()
do
	local VertexShader = Graphics.Shader.new(Graphics.ShaderTypes.VertexShader)
	local FragmentShader = Graphics.Shader.new(Graphics.ShaderTypes.FragmentShader)

	VertexShader:SetShaderSource([[

		layout(std140) uniform CameraTransformation {
			mat4 Projection, View;
		};
		uniform mat4 Model;
		
		in vec3 aPos;
		
		void main() {
			gl_Position = Projection * View * mat4(1.0) * vec4(aPos, 1.0);
		}
	]])

	FragmentShader:SetShaderSource([[
		
		out vec3 OutColor;
		
		void main() {
			OutColor = vec3(0.0, 1.0, 0.0);
		}
	]])

	QuadProgram:AttachShader(VertexShader)
	QuadProgram:AttachShader(FragmentShader)

	local ErrorLog = QuadProgram:Link()
	if ErrorLog then
		warn("Failed to link QuadProgram, OpenGL info log:\n", ErrorLog)
	end

	QuadProgram:DetachShader(VertexShader)
	QuadProgram:DetachShader(FragmentShader)

	QuadProgram:SetUniformBlockBinding(QuadProgram:GetUniformBlockIndex("CameraTransformation"), 0)
end


local ForwardInput = Input.GetBinding("Forward", Enums.Scancode.W)
local LeftInput = Input.GetBinding("Left", Enums.Scancode.A)
local RightInput = Input.GetBinding("Right", Enums.Scancode.D)
local BackInput = Input.GetBinding("Back", Enums.Scancode.S)

function Camera:Update()

	self.Position = self.Position + self.LookDirection * ((ForwardInput:Held() and 1 or 0) - (BackInput:Held() and 1 or 0))
	--self.Position += self.LookDirection * Vector3.new((ForwardInput:Held() and 1 or 0) - (BackInput:Held() and 1 or 0))
	
	CameraTransformationUniformBuffer:Bind()
	Graphics.UniformBuffer.CopyToPointer(Mat4.LookAt(self.Position, self.Position + self.LookDirection, self.UpVector), 0)
	Graphics.UniformBuffer.CopyToPointer(Mat4.Perspective(math.rad(self.FOV), 800 / 600, 0.1, 1000), Mat4.ByteSize)
	--Graphics.UniformBuffer.Unbind()
end

function Camera:Draw()
	
	VAO:Bind()
	CameraTransformationUniformBuffer:BindToUniformBlockIndex(0)

	QuadProgram:Use()

	Graphics.DrawElements(Graphics.DrawModes.Triangles, 3, Graphics.Types.Uint8)

	--Graphics.VertexArray.Unbind()
end


return Camera