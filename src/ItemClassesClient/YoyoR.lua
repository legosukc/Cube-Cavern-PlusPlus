
local class = {}

class.Name = "YoyoR"
class.Description
= [[4 melee damage.
Hold down UsePrimary to charge its sling distance. Let go and guide it with where you're looking. String damages on contact and pierces.
- 25% longer wind-up
- 20% less range
- 30% less responsiveness
RED modifier:
Each enemy hit by the string adds to the POTENCY counter.
When dropkicking the Yoyo when it retracts, it trades single high damage from the base Yoyo for medium splash damage.
Damage and splash size increases from POTENCY.
- Cooldown after dropkicking Yoyo is now 8 seconds instead of 6.
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