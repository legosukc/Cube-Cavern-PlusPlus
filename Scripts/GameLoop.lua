--!noreturn

local Game = Game


local HookedFunctions = {}

function Game.HookToLoop(HookFunction)
	
	

	table.insert(
end

function Game.Update()
	local Game = Game
	
	Game.Items.Update()
	Game.Player.Update()
	Game.Enemies.Update()
end

function Game.Draw()
	local Game = Game
	
	Game.Items.Draw()
	Game.Player.Draw()
	Game.Enemies.Draw()
end