
local class = {}

class.Name = "Confidence."
class.Description
= [[At a third of your health, overall damage is increased by 25%.
On death, your confidence will shatter and will apply 'Humbled' for the remainder of the room.
You regain confidence after each floor.
On Humbled:
+ Damage is increased by 50%
- Your health is replaced with EGO.
- You can take 4 hits before EGO shatters and you die.
? After 'Humbled' runs out, your health will be set to half.
]]
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