
local Camera = Game.Camera
local Graphics = Game.Graphics
local Input = Game.Input


local Player = {
	Position = Vector3.new();
	Velocity = Vector3.new();

	Grounded = false;
	Crawling = false;

	Dropkicking = false;
	BaseDropkickVelocityMul = 6;
	DropkickVelocityMul = SyncedModifier.new();

	ThirdPerson = false;

	DropkickChain = 0;
	BaseMaxDropkickChain = 3;
	MaxDropkickChain = SyncedModifier.new();

	JumpTimer = 0;

	StopSpeed = SyncedModifier.new();
	BaseStopSpeed = 0.01;

	Gravity = SyncedModifier.new();
	BaseGravity = 0.05;

	GroundFriction = SyncedModifier.new();
	BaseGroundFriction = 0.075;

	AirFriction = SyncedModifier.new();
	BaseAirFriction = 0.01;

	Health = 5;
	MaxHealth = SyncedModifier.new();
	BaseMaxHealth = 5;

	Mana = 2;
	MaxMana = SyncedModifier.new();
	BaseMaxMana = 2;
}

do
	local ViewportArmVAO = Graphics.VertexArray.new()
	local ViewportArmVBO = Graphics.VertexBuffer.new()
	local ViewportArmEBO = Graphics.ElementBuffer.new()

	ViewportArmVAO:Bind()
	ViewportArmVBO:Bind()
	ViewportArmEBO:Bind()

	local ViewportArmModel = ModelLoader.LoadFile("Models\\ViewportArm.obj")

	Graphics.VertexBuffer.CopyFromBuffer(ViewportArmModel.VertexData)
	Graphics.ElementBuffer.CopyFromBuffer(ViewportArmModel.IndexData)

	Graphics.VertexArray.EnableAttribute(0)
	Graphics.VertexArray.SetAttributePointer(0, 3, Graphics.Types.Float, false, ViewportArmModel.StrideSize)

	Graphics.VertexArray.EnableAttribute(1)
	Graphics.VertexArray.SetAttributePointer(1, 3, Graphics.Types.Float, false, ViewportArmModel.StrideSize)

	Graphics.VertexArray.EnableAttribute(2)
	Graphics.VertexArray.SetAttributePointer(2, 2, Graphics.Types.Float, false, ViewportArmModel.StrideSize)

	Graphics.VertexArray.Unbind()


	local ViewportArmProgram = Graphics.Program.new()

	do
		local VertexShader = Graphics.Shader.new(Graphics.ShaderTypes.VertexShader)
		local FragmentShader = Graphics.Shader.new(Graphics.ShaderTypes.FragmentShader)

		VertexShader:SetShaderSource([[

			attribute vec3 aPos;
			attribute vec3 aNormal;
			attribute vec2 aTexcoord;

			varying vec3 Normal;
			varying vec2 Texcoord;

			uniform mat4 Model;
			uniform mat4 Projection;
		
			void main() {
				gl_Position = Projection * mat4(1.0) * vec4(aPos + vec3(-0.4, -0.1, 0.2), 1.0);
				Normal = aNormal;
				Texcoord = aTexcoord;
			}
		]])

		local CompileError = VertexShader:Compile()
		if CompileError then
			warn("Failed to compile VertexShader. ", CompileError)
		end

		FragmentShader:SetShaderSource([[

			//uniform sampler2D ArmTexture;
			
			varying vec3 Normal;
			varying vec2 Texcoord;

			void main() {
  				/*
				vec3 norm = normalize(Normal);
				vec3 lightDir = normalize(lightPos - FragPos);
				
				gl_FragColor = vec4(
					(
						vec3(0.1)	// ambient
						+ max(dot(norm, lightDir), 0.0) * lightColor	// diffuse
						+ (0.5 * pow(max(dot(normalize(viewPos - FragPos), reflect(-lightDir, norm)), 0.0), 32) * lightColor)	// specular
					) * texture2D(ArmTexture, Texcoord).rgb, 1.0
				);*/
				gl_FragColor = vec4(1.0); //texture2D(ArmTexture, Texcoord);
			}
		]])

		CompileError = FragmentShader:Compile()
		if CompileError then
			warn("Failed to compile FragmentShader. ", CompileError)
		end

		ViewportArmProgram:AttachShader(VertexShader)
		ViewportArmProgram:AttachShader(FragmentShader)
	
		local ErrorLog = ViewportArmProgram:Link()
		if ErrorLog then
			warn("Failed to link ViewportArmProgram, OpenGL info log:\n", ErrorLog)
		end

		ViewportArmProgram:DetachAllShaders()
	end

	ViewportArmProgram:Use()
	local ViewportArmModelUniform = Graphics.Program.GetUniformLocation(ViewportArmProgram, "Model")
	local ViewportArmProjectionUniform = Graphics.Program.GetUniformLocation(ViewportArmProgram, "Projection")

	Player.GraphicsObjects = {
		ViewportLeftArmVAO = ViewportArmVAO;
		ViewportLeftArmVBO = ViewportArmVBO;
		ViewportLeftArmEBO = ViewportArmEBO;

		ViewportRightArmVAO = ViewportArmVAO;
		ViewportRightArmVBO = ViewportArmVBO;
		ViewportRightArmEBO = ViewportArmEBO;

		ViewportLeftArmProgram = ViewportArmProgram;
		ViewportLeftArmModelUniform = ViewportArmModelUniform;
		ViewportLeftArmProjectionUniform = ViewportArmProjectionUniform;

		ViewportRightArmProgram = ViewportArmProgram;
		ViewportRightArmModelUniform = ViewportArmModelUniform;
		ViewportRightArmProjectionUniform = ViewportArmProjectionUniform;
	}
end


local ForwardInput = Input.GetBinding("Forward", Enums.Scancode.W)
local LeftInput = Input.GetBinding("Left", Enums.Scancode.A)
local RightInput = Input.GetBinding("Right", Enums.Scancode.D)
local BackInput = Input.GetBinding("Back", Enums.Scancode.S)

local JumpInput = Input.GetBinding("Jump", Enums.Scancode.Space)
local CrawlInput = Input.GetBinding("Crawl", Enums.Scancode.LeftCtrl)

local DashInput = Input.GetBinding("Crawl", Enums.Scancode.LeftShift)
local DropkickInput = Input.GetBinding("Dropkick", Enums.Scancode.C)

function Player:Update()

	--self = Player

	--self.Velocity.X
	--print("player type:", typeof(self))

	local Friction = self.Grounded and self.GroundFriction or self.AirFriction


	local Velocity = self.Velocity
	if Velocity.X ~= 0 then
		Velocity.X = Velocity.X > 0 and -Friction or Friction
	end

	--Velocity.Y = Velocity.Y - self.Gravity;

	if Velocity.Z ~= 0 then
		Velocity.Z = Velocity.Z > 0 and -Friction or Friction
	end
	
	self.Crawling = CrawlInput:Held()
	if self.Crawling and JumpInput:Pressed() then
		-- TODO: implement rolling
	end

	if DropkickInput:Pressed() and not self.Dropkicking then
		self.Dropkicking = true
		print("dropkicked")
		Velocity = Velocity + Game.Camera.LookDirection * (self.BaseDropkickVelocityMul + self.DropkickVelocityMul:GetResult())
	end

	if JumpInput:Held() and self.Grounded and not self.Crawling then

		self.Grounded = false
		Velocity.Y = 3
	end

	Velocity = Velocity + Game.Camera.LookDirection * ((ForwardInput:Held() and 1 or 0) - (BackInput:Held() and 1 or 0))

	self.Velocity = Velocity
	self.Position = self.Position + Velocity
end


function Player:Draw()

	local GraphicsObjects = self.GraphicsObjects
	
	if self.ThirdPerson then

	else
		GraphicsObjects.ViewportLeftArmVAO:Bind()

		GraphicsObjects.ViewportLeftArmProgram:Use()

		--Graphics.Program.SetUniformMat4(Player.GraphicsObjects.ViewportLeftArmModelUniform, false, )
		Graphics.Program.SetUniformMat4(Player.GraphicsObjects.ViewportLeftArmProjectionUniform, false, Camera.ProjectionMatrix)

		Graphics.DrawElements(Graphics.DrawModes.Triangles, 3, Graphics.Types.Uint32)


		GraphicsObjects.ViewportRightArmVAO:Bind()

		GraphicsObjects.ViewportRightArmProgram:Use()
		Graphics.Program.SetUniformMat4(Player.GraphicsObjects.ViewportRightArmProjectionUniform, false, Camera.ProjectionMatrix)

		Graphics.DrawElements(Graphics.DrawModes.Triangles, 3, Graphics.Types.Uint32)

		Graphics.VertexArray.Unbind()
	end
end



function Player.CameraHandlerFunc(Camera)
	--[[
	local Position = Camera.Position

	local RightVector = Camera.RightVector
	local UpVector = Camera.UpVector
	local LookDirection = Camera.LookDirection

	local Yaw = Camera.Yaw
	local Pitch = Camera.Pitch

	Yaw = (Yaw + Input.MouseDelta.X * 0.2) % 360
	Pitch = math.clamp(Pitch - Input.MouseDelta.Y * 0.2, -89.9, 89.9)


	local YawRad = math.rad(Yaw)
	local PitchRad = math.rad(Pitch)

	LookDirection.X = math.cos(YawRad) * math.cos(PitchRad)
    LookDirection.Y = math.sin(PitchRad)
    LookDirection.Z = math.sin(YawRad) * math.cos(PitchRad)
	LookDirection = LookDirection:Normalize()

	-- normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	RightVector = LookDirection:Cross(Vector3.new(0, 1, 0)):Normalize()
    UpVector    = RightVector:Cross(LookDirection):Normalize()

	Position = Position + LookDirection * ((ForwardInput:Held() and 0.1 or 0) - (BackInput:Held() and 0.1 or 0))
	Position = Position + RightVector * ((RightInput:Held() and 0.1 or 0) - (LeftInput:Held() and 0.1 or 0))

	
	--CameraTransformationUniformBuffer:Bind()
	--Graphics.UniformBuffer.CopyToPointer(Mat4.LookAt(Position, Position + LookDirection, UpVector), 0)
	--Graphics.UniformBuffer.CopyToPointer(Mat4.Perspective(math.rad(self.FOV), 800 / 600, 0.1, 1000), Mat4.ByteSize)
	--Graphics.UniformBuffer.Unbind()

	Camera.ProjectionMatrix = Mat4.Perspective(math.rad(self.FOV), 800 / 600, 0.1, 1000)
	Camera.ViewMatrix = Mat4.LookAt(Position, Position + LookDirection, UpVector)

	Camera.Yaw = Yaw
	Camera.Pitch = Pitch

	Camera.Position = Position
	Camera.LookDirection = LookDirection
	Camera.RightVector = RightVector
	Camera.UpVector = UpVector]]

	local Yaw = (Camera.Yaw + Input.MouseDelta.X * 0.2) % 360
	local Pitch = math.clamp(Camera.Pitch - Input.MouseDelta.Y * 0.2, -89.9, 89.9)


	local YawRad = math.rad(Yaw)
	local PitchRad = math.rad(Pitch)

	local LookDirection = Vector3.new(
		math.cos(YawRad) * math.cos(PitchRad),
		math.sin(PitchRad),
		math.sin(YawRad) * math.cos(PitchRad)
	):Normalize()

	-- normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	local RightVector = LookDirection:Cross(Vector3.new(0, 1, 0)):Normalize()
    local UpVector    = RightVector:Cross(LookDirection):Normalize()

	local CameraPosition = Player.Position

	Camera.ProjectionMatrix = Mat4.Perspective(math.rad(self.FOV), 800 / 600, 0.1, 1000)
	Camera.ViewMatrix = Mat4.LookAt(CameraPosition, CameraPosition + LookDirection, UpVector)

	Camera.Yaw = Yaw
	Camera.Pitch = Pitch

	Camera.Position = CameraPosition
	Camera.LookDirection = LookDirection
	Camera.RightVector = RightVector
	Camera.UpVector = UpVector
end

Camera.CameraHandlerFunc = Player.CameraHandlerFunc

return Player