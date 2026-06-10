
local Camera = Game.Camera
local Graphics = Game.Graphics
local Input = Game.Input


local Player = {
	Position = Vector3.new();
	Velocity = Vector3.new();

	-- States
	Grounded = false;
	Crawling = false;
	Dropkicking = false;

	ThirdPerson = false;

	DropkickChain = 0;
	MaxDropkickChain = 3;

	JumpTimer = 0;

	StopSpeed = 0.01;
	Gravity = 0.05;
	GroundFriction = 0.075;
	AirFriction = 0.01;

	Health = 5;
	MaxHealth = 5;

	Mana = 2;
	MaxMana = 2;
}

do
	local ViewportArmVAO = Graphics.VertexArray.new()
	local ViewportArmVBO = Graphics.VertexBuffer.new()
	local ViewportArmEBO = Graphics.ElementBuffer.new()

	ViewportArmVAO:Bind()
	ViewportArmVBO:Bind()
	ViewportArmEBO:Bind()

	local ViewportArmModel = ModelLoader.LoadFile("Models\ViewportArm.obj")

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
				gl_Position = Projection * Model * vec4(aPos, 1.0);
				Normal = aNormal;
				Texcoord = aTexcoord;
			}
		]])

		local CompileError = VertexShader:Compile()
		if CompileError then
			warn("Failed to compile VertexShader. ", CompileError)
		end

		FragmentShader:SetShaderSource([[

			uniform sampler2D ArmTexture;
			
			varying vec3 Normal;
			varying vec2 Texcoord;

			void main() {
  	
				vec3 norm = normalize(Normal);
				vec3 lightDir = normalize(lightPos - FragPos);

				gl_FragColor = vec4(
					(
						vec3(0.1)	// ambient
						+ max(dot(norm, lightDir), 0.0) * lightColor	// diffuse
						+ (0.5 * pow(max(dot(normalize(viewPos - FragPos), reflect(-lightDir, norm)), 0.0), 32) * lightColor)	// specular
					) * texture2D(ArmTexture, Texcoord).rgb, 1.0
				);
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

	Player.GraphicsObjects = {
		ViewportLeftArmVAO = ViewportArmVAO;
		ViewportLeftArmVBO = ViewportArmVBO;
		ViewportLeftArmEBO = ViewportArmEBO;

		ViewportRightArmVAO = ViewportArmVAO;
		ViewportRightArmVBO = ViewportArmVBO;
		ViewportRightArmEBO = ViewportArmEBO;

		ViewportLeftArmProgram = ViewportArmProgram;
		ViewportRightArmProgram = ViewportArmProgram;
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
		self.Velocity.X = Velocity.X > 0 and -Friction or Friction
	end

	self.Velocity.Y = Velocity.Y - self.Gravity;

	if Velocity.Z ~= 0 then
		self.Velocity.Z = Velocity.Z > 0 and -Friction or Friction
	end
	
	self.Crawling = CrawlInput:Held()
	if self.Crawling and JumpInput:Pressed() then
		-- TODO: implement rolling
	end

	if JumpInput:Held() and self.Grounded and not self.Crawling then

		self.Grounded = false
		self.Velocity.Y = 5
	end

	self.Velocity = self.Velocity + Game.Camera.LookDirection * ((ForwardInput:Held() and 1 or 0) - (BackInput:Held() and 1 or 0))


	self.Position = self.Position + self.Velocity
end


function Player:Draw()

	local GraphicsObjects = self.GraphicsObjects
	
	if self.ThirdPerson then

	else
		GraphicsObjects.ViewportLeftArmVAO:Bind()

		GraphicsObjects.ViewportLeftArmProgram:Use()

		Graphics.DrawElements(Graphics.DrawModes.Triangles, 3, Graphics.Types.Uint32)


		GraphicsObjects.ViewportRightArmVAO:Bind()

		GraphicsObjects.ViewportRightArmProgram:Use()

		Graphics.DrawElements(Graphics.DrawModes.Triangles, 3, Graphics.Types.Uint32)

		Graphics.VertexArray.Unbind()
	end
end


return Player