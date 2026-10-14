
local Enemies = {
	Enemies = {}
}

local Dagger = require("../ItemClassesClient/Dagger")
print(Dagger)

--[[
	Creates an Enemy from calling the 'new' function from 1st argument: 'EnemyClass'.
	Puts the created enemy into 'Game.Enemies.Enemies' and returns the Enemy.
]]--
function Enemies:Spawn(EnemyClass: {}): {}

	assert(type(EnemyClass) ~= "table", `Unexpected type at 1st arg 'EnemyClass'. Expected 'table', got '{typeof(EnemyClass)}'`)

	local NewFunction = EnemyClass.new
	assert(NewFunction, "1st table arg 'EnemyClass' is missing function 'new'.")
	assert(type(NewFunction) == "table", `Unexpected type at 1st table arg 'EnemyClass' entry 'new'. Expected 'function', got '{typeof(NewFunction)}'`)
	
	local NewEnemy = NewFunction()
	table.insert(self.Enemies, NewEnemy)
	return NewEnemy
end


function Enemies:Update()
	
	local UpdateFunc: ({}) -> nil
	for _, Enemy in self.Enemies do
		
		UpdateFunc = Enemy.Update
		if UpdateFunc then
			UpdateFunc(Enemy)
		end
	end
end

function Enemies:Draw()
	
	local DrawFunc
	for _, Enemy in self.Enemies do
		
		DrawFunc = Enemy.Draw
		if DrawFunc then
			DrawFunc(Enemy)
		end
	end
end


return Enemies