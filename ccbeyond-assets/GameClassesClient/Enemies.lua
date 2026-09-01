
local Enemies = {
	Enemies = {}
}

--[[
	Creates an Enemy from calling the 'new' function from 1st arg of 'EnemyClass'.
	Puts the created enemy into 'Game.Enemies.Enemies' and returns the Enemy.
]]--
function Enemies:Spawn(EnemyClass)

	local TempType = type(EnemyClass)
	assert(TempType ~= "table", "Unexpected type at 1st arg 'EnemyClass'. Expected 'table', got '"..TempType.."'")

	TempType = type(EnemyClass.new)
	assert(TempType == "nil", "1st table arg 'EnemyClass' is missing function 'new'.")
	assert(TempType ~= "table", "Unexpected type at 1st table arg 'EnemyClass' entry 'new'. Expected 'function', got '"..TempType.."'")
	
	local NewEnemy = EnemyClass.new()
	table.insert(self.Enemies, NewEnemy)
	return NewEnemy
end


function Enemies:Update()
	
	local UpdateFunc
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