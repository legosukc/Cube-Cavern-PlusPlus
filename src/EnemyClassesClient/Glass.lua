
local ParentClass = Assets.BaseClassesClient.EnemyBase

local class = {}
class.__index = class
setmetatable(class, ParentClass)

class.Name = "Glass"
class.BookEntry = "Breaks in one-hit to ANYTHING. Makes nearby enemies target the destroyer, also drops Glass Shard on destruction."

class.Health = 1



function class.new()
	
	local new = ParentClass.new()
	return setmetatable(new, class)
end


function class:OnHurt(DamageTaken)
	self.Dead = true;
	self.Health = 0
end

function class:Damage()
end


function class:Update()
end

function class:Draw()
end

return class