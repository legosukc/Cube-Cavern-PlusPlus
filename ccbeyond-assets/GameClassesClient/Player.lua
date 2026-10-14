local Camera = Game.Camera
local Graphics = Game.Graphics
local Input = Game.Input

local Texture = Graphics.Texture

local SyncedModifierCollection = Assets.MiscClassesGlobal.SyncedModifierCollection

local Player = {
	Position = Vector3.new();
	Velocity = Vector3.new();

	MoveDirection = Vector3.new(0, 0, 0);

	JumpTimer = 0;
	StartingJumpTimer = 0;
	JumpTimerMul = SyncedModifierCollection.new();
	BaseJumpTimer = 1.8;

	Grounded = true;
	Crawling = false;

	Sliding = false;
	SlideCooldown = 0;
	SlideStopCooldownMul = SyncedModifierCollection.new();
	SlideStopBaseCooldown = 1.6;
	SlideInitialVelocity = nil;

	Dropkicking = false;
	BaseDropkickVelocityMul = 6;
	DropkickVelocityMul = SyncedModifierCollection.new();

	ThirdPerson = false;

	DropkickChain = 0;
	BaseMaxDropkickChain = 3;
	MaxDropkickChain = SyncedModifierCollection.new();

	StopSpeed = SyncedModifierCollection.new();
	BaseStopSpeed = 0.01;

	GravityMul = SyncedModifierCollection.new();
	BaseGravity = 0.1;

	GroundFriction = SyncedModifierCollection.new();
	BaseGroundFriction = 0.9;

	AirFriction = SyncedModifierCollection.new();
	BaseAirFriction = 0.01;

	AirControl = SyncedModifierCollection.new();

	DropkickAirControlMul = SyncedModifierCollection.new();
	DropkickAirControlBaseMul = 0.75;

	Health = 5;
	MaxHealth = SyncedModifierCollection.new();
	BaseMaxHealth = 5;

	Mana = 2;
	MaxMana = SyncedModifierCollection.new();
	BaseMaxMana = 2;

	LeftViewportArmVisible = true;
	LeftViewportArmOffset = Vector3.new(-0.15, -0.175, -0.3);
	LeftViewportArmAngle = Vector3.new(0, 0, 0);

	RightViewportArmVisible = true;
	RightViewportArmOffset = Vector3.new(0.15, -0.175, -0.3);
	RightViewportArmAngle = Vector3.new(0, 0, 0);

	ViewarmRenderObject = Assets.RenderObjectsClient.rendViewarm;
}

Player.Bindings = {
	ForwardInput = Input.GetBinding("Forward", Enums.Scancode.W);
	LeftInput = Input.GetBinding("Left", Enums.Scancode.A);
	RightInput = Input.GetBinding("Right", Enums.Scancode.D);
	BackInput = Input.GetBinding("Back", Enums.Scancode.S);

	JumpInput = Input.GetBinding("Jump", Enums.Scancode.Space);
	SlideInput = Input.GetBinding("Slide", Enums.Scancode.LeftShift);

	DashInput = Input.GetBinding("Dash", Enums.Scancode.C);
	DropkickInput = Input.GetBinding("Dropkick", Enums.Scancode.C, Enums.Scancode.LeftCtrl);

	UsePrimaryInput = Input.GetBinding("UsePrimary", Enums.Scancode.LeftClick);
	UseSecondaryInput = Input.GetBinding("UseSecondary", Enums.Scancode.RightClick, Enums.Scancode.LeftAlt);
}



function Player:UsePrimary()
	local HeldItem = Game.Inventory.HeldItem
	if not HeldItem then
		return
	end

	HeldItem:UsePrimary()
end

function Player:UseSecondary()
	local HeldItem = Game.Inventory.HeldItem
	if not HeldItem then
		return
	end

	HeldItem:UseSecondary()
end


function Player:Move(MoveDirection)
	--self.Velocity += MoveDirection * (self.Grounded and 0.1625 or ((self.Dropkicking and ((self.DropkickAirControlMul:GetResult() or 0) + 1) or 1) * 0.09 + (self.AirControl:GetResult() or 0)))

	local wishspeed = 3
    local accel = self.Grounded and 7 or 3.5

    local currentspeed = self.Velocity:Dot(MoveDirection)
    local addspeed = wishspeed - currentspeed
    if addspeed <= 0 then
        return
    end
    local accelspeed = math.min(accel * Game.DeltaTime * wishspeed, addspeed)
    
    self.Velocity += MoveDirection * accelspeed
end

function Player:ApplyGroundFriction()
	local Friction = self.Sliding and 0.98 or 0.825
	self.Velocity.X *= Friction
	self.Velocity.Z *= Friction
end

function Player:ApplyAirFriction()
	--[[
	local Velocity = self.Velocity

	local VelocityNoY = Velocity:StripY()
	local NewVelocity = VelocityNoY - (VelocityNoY:Normalize():StripNaN() * (self.BaseAirFriction + (self.AirFriction:GetResult() or 0)))
	self.Velocity = VelocityNoY:Magnitude() < NewVelocity:Magnitude() and Vector3.new(0, Velocity.Y, 0) or Vector3.new(NewVelocity.X, Velocity.Y, NewVelocity.Z)
	]]
end

function Player:Jump()
	if self.Bindings.JumpInput:Pressed() and self.Grounded then
		self.Grounded = false
		self.Crawling = false
		if self.Sliding then
			self.Sliding = false
			self.SlidingCooldown = self.SlideStopBaseCooldown * (self.SlideStopCooldownMul:GetResult() or 0)
			self.Velocity = self.Velocity:StripY():Normalize():StripNaN()
		end
		self.Velocity.Y = 1.4
		self.JumpTimer = self.BaseJumpTimer * (1 + (self.JumpTimerMul:GetResult() or 0))
		self.StartingJumpTimer = self.JumpTimer
	end
end

function Player:Update()

	local Bindings = self.Bindings

	local MoveDirection = (
		(Camera.LookDirection * ((Bindings.ForwardInput:Held() and 1 or 0) - (Bindings.BackInput:Held() and 1 or 0))):StripY():Normalize():StripNaN()
		+ (Camera.RightVector * ((Bindings.RightInput:Held() and 1 or 0) - (Bindings.LeftInput:Held() and 1 or 0))):StripY():Normalize():StripNaN()
	):Normalize():StripNaN()
	self.MoveDirection = MoveDirection

	self[self.Grounded and "ApplyGroundFriction" or "ApplyAirFriction"](self);
	
	if Bindings.SlideInput:Held() then
		-- TODO: implement rollinga
	end

	if Bindings.DropkickInput:Pressed() and not self.Dropkicking then
		self.Dropkicking = true
		if self.Grounded then
			self.Velocity.Y = 2
			self.Grounded = false
		end
		print("dropkicked")
		self.Velocity = Camera.LookDirection * self.Velocity:Magnitude() --* (self.BaseDropkickVelocityMul + (self.DropkickVelocityMul:GetResult() or 0))
	end

	do
		local ApplyGravity = self.BaseGravity * (1 + (self.GravityMul:GetResult() or 0))
		if self.JumpTimer > 0 then
			if Bindings.JumpInput:Pressed() then
				self.JumpTimer = math.max(self.JumpTimer - (Game.DeltaTime * 5), 0);
				ApplyGravity *= 1 - (self.JumpTimer / self.StartingJumpTimer)
			else
				self.JumpTimer = 0
			end
		end
		self.Velocity.Y -= ApplyGravity
	end

	self:Move(MoveDirection)

	self:Jump()

	if Bindings.UsePrimaryInput:Pressed() then
		self:UsePrimary()
	end
	if Bindings.UseSecondaryInput:Pressed() then
		self:UseSecondary()
	end


	--[[
	if not self.Sliding then
		self.Velocity += MoveDirection * (self.Grounded and 0.1625 or ((self.Dropkicking and ((self.DropkickAirControlMul:GetResult() or 0) + 1) or 1) * 0.09 + (self.AirControl:GetResult() or 0)))
	end]]

	self.Position += self.Velocity * Game.DeltaTime * 60

	if self.Position.Y < 0 then
		self.Velocity.Y = 0
		self.Position.Y = 0
		self.Grounded = true
		self.JumpTimer = 0
		self.Dropkicking = false
	else
		self.Grounded = false
	end

	--print(`Position: {self.Position}\nVelocity: {Velocity}`)
end


--local SetUniformVec3 = Graphics.Program.SetUniformVec3
--local SetUniformMat4 = Graphics.Program.SetUniformMat4

local VelocityLerp = Player.Velocity
local ZLookLerp = Camera.LookDirection:Dot(Camera.WorldUpVector) * 0.05
function Player:Draw()

	VelocityLerp = VelocityLerp:Lerp(self.Velocity * 0.01, 0.2)

	local GraphicsObjects = self.GraphicsObjects
	
	if self.ThirdPerson then

	else
		ZLookLerp = math.lerp(ZLookLerp, Camera.LookDirection:Dot(Camera.WorldUpVector) * 0.05, 0.15)
		local ArmOffset = Vector3.new(
			-VelocityLerp:Dot(Camera.RightVector),
        	-VelocityLerp:Dot(Camera.UpVector),
        	VelocityLerp:Dot(Camera.LookDirection) + ZLookLerp
		)

		local LeftArmOffset = self.LeftViewportArmOffset + ArmOffset
		local RightArmOffset = self.RightViewportArmOffset + ArmOffset
	
		do
			local YawRad = math.rad(self.LeftViewportArmAngle.X)
			local PitchRad = math.rad(self.LeftViewportArmAngle.Y)

			local LookDirection = Vector3.new(
				math.cos(YawRad) * math.cos(PitchRad),
				math.sin(PitchRad),
				math.sin(YawRad) * math.cos(PitchRad)
			):Normalize()

			local RightVector = LookDirection:Cross(Vector3.new(0, 1, 0)):Normalize()
    		local UpVector    = RightVector:Cross(LookDirection):Normalize()
			ViewMatrix = Mat4.LookAt(LeftArmOffset, LeftArmOffset + LookDirection, UpVector)
		end

		if self.LeftViewportArmVisible then
			
			self.ViewarmRenderObject:Draw(nil, LeftArmOffset)
		end

		if self.RightViewportArmVisible then
			
			self.ViewarmRenderObject:Draw(nil, RightArmOffset)
		end
		Graphics.VertexArray.Unbind()

		local HeldItem = Game.Inventory.HeldItem
		if HeldItem then
			HeldItem:DrawViewmodel(LeftArmOffset, RightArmOffset)
		end
	end
end



function Player.CameraHandlerFunc(Camera)

	-- TODO: make it so camera input properly follows the world up vector (instead of only the y component)
	local Yaw = (Camera.Yaw + Input.MouseDeltaX * 0.4 * Camera.WorldUpVector.Y) % 360
	local Pitch = math.clamp(Camera.Pitch - Input.MouseDeltaY * 0.4 * Camera.WorldUpVector.Y, -89.9, 89.9)

	local YawRad = math.rad(Yaw)
	local PitchRad = math.rad(Pitch)

	local LookDirection = Vector3.new(
		math.cos(YawRad) * math.cos(PitchRad),
		math.sin(PitchRad),
		math.sin(YawRad) * math.cos(PitchRad)
	):Normalize()

	-- normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	local RightVector = LookDirection:Cross(Camera.WorldUpVector):Normalize()
    local UpVector    = RightVector:Cross(LookDirection):Normalize()

	local CameraPosition = Player.Position

	Camera.ProjectionMatrix = Mat4.Perspective(math.rad(Camera.FOV), Graphics.AspectRatio, 0.1, 1000)
	Camera.ViewMatrix = Mat4.LookAt(CameraPosition, CameraPosition + LookDirection, UpVector)

	Camera.Yaw = Yaw
	Camera.Pitch = Pitch

	Camera.Position = CameraPosition
	Camera.LookDirection = LookDirection
	Camera.RightVector = RightVector
	Camera.UpVector = UpVector
end

Camera.ActiveCameraHandler = Player.CameraHandlerFunc

return Player