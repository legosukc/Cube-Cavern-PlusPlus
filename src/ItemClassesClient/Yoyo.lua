
local class = {}

class.Name = "Yoyo"
class.Description = "3 melee damage. Hold down UsePrimary to charge its sling distance. Let go and guide it with where you're looking. String damages on contact and pierces."
class.BookEntry = "chuddy forgot to set the book description"

class.__index = class


function class.new()
	
	local new = {}
	setmetatable(new, class)

	return new
end


function class:UsePrimary()
end

function class:UseSecondary()
end


function class:Update()
end

function class:Draw()
end


function class:OnEquip()
end

function class:OnUnequip()
end


return class