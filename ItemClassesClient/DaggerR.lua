
local class = {}

class.Name = "DaggerR"
class.Description = "chuddy forgot to set the description"
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