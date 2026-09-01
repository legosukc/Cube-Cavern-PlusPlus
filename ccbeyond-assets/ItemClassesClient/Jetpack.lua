
local class = {}

class.Name = "Jetpack"
class.Description = [[Press keybind 'Dash' to quickly launch yourself a short distance in the direction you're moving, good for getting rid of speed and dodging. Grants 4 extra inventory slots. "Whatd'ya get say, fast jet!"]]
class.ItemType = "Equipment"
class.ItemSubtype = "Backpack"
class.BookEntry = "Back Equipment. Grants 3 extra inventory slots. It can be crafted with a 'Stick', 'Rope', and 'Cloth'."

-- How much charge is added per second
class.RechargeRate = 1
class.MaxCharge = 1

class.__index = class


function class.new()
	return setmetatable(
		{
			Charge = class.MaxCharge
		},
		class
	)
end


function class:UsePrimary()
end

function class:UseSecondary()
end


function class:Update()
	self.Charge = math.min(self.Charge + self.RechargeRate * Game.DeltaTime, self.MaxCharge)
end

function class:Draw()
end


function class:OnEquip()
end

function class:OnUnequip()
end


return class