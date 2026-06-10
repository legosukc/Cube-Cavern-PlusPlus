
local ParentClass = Assets.BaseClassesClient.EnemyBase

local class = {}
class.__index = class
setmetatable(class, ParentClass)

class.Name = "Poopie"
class.BookEntry = "it shits. don't touch the shit."

class.Health = 4



function class.new()
	
	local new = ParentClass.new()
	new.PoopTimer = 0

	return setmetatable(new, class)
end


function class:OnHurt(DamageTaken)
end

function class:Damage()
end


function class:Update()
end

function class:Draw()
end

return class