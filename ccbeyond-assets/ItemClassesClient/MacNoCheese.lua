
local class = {}

class.Name = "Mac-No-Cheese"
class.Description = "Dual sub-machine blasters with 30 rounds total. 5 ranged damage and extremely high firerate. 1 ammo restored every 0.5 seconds. Great for finishing larger enemies. Don't act like I won't do it. My ops are gonna chew it."
class.BookEntry = ""

class.__index = class


function class.new()
	
	local new = {
		ClipAmmo = 30;
		LastInteractTime = time();
	}
	setmetatable(new, class)

	return new
end


function class:UsePrimary()

	if self.ClipAmmo < 1 then
		return
	end
	self.ClipAmmo = self.ClipAmmo - 1
	self.LastInteractTime = time()

	print("pew")
end

function class:UseSecondary()
end


function class:Update()

	local LastInteractTimeDif = time() - self.LastInteractTime
	if self.ClipAmmo >= 30 or LastInteractTimeDif < 0.5 then
		return
	end

	self.ClipAmmo = self.ClipAmmo + (LastInteractTimeDif / 0.5);

	self.LastInteractTime = time()
end

function class:Draw()
end


function class:OnEquip()
end

function class:OnUnequip()
end


return class