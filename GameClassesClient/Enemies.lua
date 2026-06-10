
local Enemies = {
	Enemies = {}
}


function Enemies:Spawn(EnemyClass)
	
	local NewEnemy = EnemyClass.new()
	table.insert(self.Enemies, NewEnemy)
	return NewEnemy
end


function Enemies:Update()
	
	for _, Enemy in ipairs(self.Enemies) do
		
		if Enemy.Update then
			Enemy:Update()
		end
	end
end

function Enemies:Draw()
	
	for _, Enemy in ipairs(self.Enemies) do
		
		if Enemy.Draw then
			Enemy:Draw()
		end
	end
end


return Enemies