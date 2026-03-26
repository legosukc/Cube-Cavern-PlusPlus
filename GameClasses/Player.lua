
local Camera = Game.Camera


local Player = {
	Position = Vector3.new();
	Velocity = Vector3.new();

	Grounded = false;
	Crawling = false;

	JumpTimer = 0;
}

local ForwardInput, LeftInput, RightInput, BackInput, JumpInput, CrawlInput;
do
	local Input = Game.Input
	
	ForwardInput = Input:GetBinding("Forward")
	LeftInput = Input:GetBinding("Left")
	RightInput = Input:GetBinding("Right")
	BackInput = Input:GetBinding("Back")

	JumpInput = Input:GetBinding("Jump")
	CrawlInput = Input:GetBinding("Crawl")
end

function Player:Update()

	--self.Velocity.X
	
	self.Crawling = CrawlInput:Held()
	if self.Crawling and JumpInput:Pressed() then
		-- TODO: implement rolling
	end

	if JumpInput:Held() and self.Grounded and not self.Crawling then

		self.Grounded = false
		self.Velocity.Y = 5
	end



	self.Position += self.Velocity
end


function Player:Draw()

end


return Player