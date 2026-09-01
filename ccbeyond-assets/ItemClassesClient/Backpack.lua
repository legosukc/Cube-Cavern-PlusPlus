
local class = {}

class.Name = "Backpack"
class.Description = "Student Starterpack. Grants 3 extra inventory slots."
class.BookEntry = "Back Equipment. Grants 3 extra inventory slots. It can be crafted with a 'Stick', 'Rope', and 'Cloth'."

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