
local class = {}

class.Name = "EnemyBase"
class.BookEntry = "chuddy forgot to set the book entry"

class.Health = 5


class.__index = class


function class.new()
	
	local new = {
		Dead = false;
	}
	setmetatable(new, class)

	return new
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