
local ParentClass = Assets.BaseClassesClient.EnemyBase

local class = {}
class.__index = class
setmetatable(class, ParentClass)

class.Name = "Poopie"
class.BookEntry = "Spits out shit that deals 2 ranged damage. It lunges around frequently between shots using its little stub arms. 4 health."

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