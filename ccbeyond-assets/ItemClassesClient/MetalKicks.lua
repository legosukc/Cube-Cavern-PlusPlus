
local LocalPlayer = Game.Players.LocalPlayer


local class = {}

class.Name = "Metal Kicks"
class.Description = [[+2 dropkick stamina and 1.5x dropkick damage, your movespeed and acceleration will be hindered. 1.5x gravity while in dropkicking, else 1.25x gravity. "It's a supersonic flight, gotta keep it goin'."]]
class.BookEntry = "chuddy forgot to set the book description"

class.__index = class


function class.new()
	return setmetatable({}, class)
end


function class:Update()
end

function class:Draw()
end


function class:OnEquip()
	
	self.DropkickStaminaModifier = LocalPlayer:ApplyStatModifier("DropkickStamina", 2)
	self.DropkickDamageMulModifier = LocalPlayer:ApplyStatModifier("DropkickDamageMul", 1.5)
	self.GravityModifier = LocalPlayer:ApplyStatModifier("Gravity", function()
		return LocalPlayer.Dropkicking and 1.5 or 1.25
	end)
end

function class:OnUnequip()

	local function DestroyModifier(ModifierName)

		local Modifier = self[ModifierName]
		if Modifier then
			Modifier:Destroy()
			self[ModifierName] = nil
		end
	end

	DestroyModifier("DropkickStaminaModifier")
	DestroyModifier("DropkickDamageMulModifier")
	DestroyModifier("GravityModifier")
end


return class