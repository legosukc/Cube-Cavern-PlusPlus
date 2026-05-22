
local Camera = Game.Camera
local Input = Game.Input


local Player = {
	Position = Vector3.new();
	Velocity = Vector3.new();

	Grounded = false;
	Crawling = false;

	JumpTimer = 0;
}


local ForwardInput = Input.GetBinding("Forward", Enums.Scancode.W)
local LeftInput = Input.GetBinding("Left", Enums.Scancode.A)
local RightInput = Input.GetBinding("Right", Enums.Scancode.D)
local BackInput = Input.GetBinding("Back", Enums.Scancode.S)

local JumpInput = Input.GetBinding("Jump", Enums.Scancode.Space)
local CrawlInput = Input.GetBinding("Crawl", Enums.Scancode.LeftShift)

function Player:Update()

	--self = Player

	--self.Velocity.X
	--print("player type:", typeof(self))
	
	self.Crawling = CrawlInput:Held()
	if self.Crawling and JumpInput:Pressed() then
		-- TODO: implement rolling
	end

	if JumpInput:Held() and self.Grounded and not self.Crawling then

		self.Grounded = false
		self.Velocity.Y = 5
	end

	if ForwardInput:Held() then
		print("forward held!")
	end

	self.Velocity = self.Velocity + Camera.LookDirection * ((ForwardInput:Held() and 1 or 0) - (BackInput:Held() and 1 or 0))


	self.Position = self.Position + self.Velocity
end


function Player:Draw()

end


return Player