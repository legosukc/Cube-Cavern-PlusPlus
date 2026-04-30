
local Camera = {
	LookDirection = Vector3.new(0, 1, 2);
}

print("camera look dir: ", Camera.LookDirection)



local VAO = Game.Graphics.VertexArray.new()
local VBO = Game.Graphics.VertexBuffer.new()
local EBO = Game.Graphics.ElementBuffer.new()

VAO:Bind()
VBO:Bind()
EBO:Bind()

do
	local QuadModelBuffer = buffer.create(32)

	QuadModelBuffer:writef32(0,  -0.5)
	QuadModelBuffer:writef32(4,  -0.5)
	QuadModelBuffer:writef32(8,   0.0)

	QuadModelBuffer:writef32(12,  0.5)
	QuadModelBuffer:writef32(16, -0.5)
	QuadModelBuffer:writef32(20,  0.0)

	QuadModelBuffer:writef32(24,  0.0)
	QuadModelBuffer:writef32(28,  0.5)
	QuadModelBuffer:writef32(32,  0.0)
end

function Camera:Update()

end


return Camera