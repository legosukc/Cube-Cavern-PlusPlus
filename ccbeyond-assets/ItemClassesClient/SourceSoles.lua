
local OriginalPlayerMoveFunc = Game.Player.Move


local class = {}

class.Name = "Sauce Soles"
class.Description = "it might even make you QUAKE..."
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


function class.Player_Move_QuakeMovement(Player, MoveDirection)
    local wishspeed = 10
    local accel = 10

    local currentspeed = Player.Velocity:Dot(MoveDirection)
    local addspeed = wishspeed - currentspeed
    if addspeed <= 0 then
        return
    end
    local accelspeed = accel * Game.DeltaTime * wishspeed
    if accelspeed > addspeed then
        accelspeed = addspeed
    end
    
    Player.Velocity += MoveDirection * accelspeed
end

function class:OnEquip()
    if self.Owner.Move ~= OriginalPlayerMoveFunc then
        return false
    end
    self.Owner.Move = self.Player_Move_QuakeMovement
    return true
end

function class:OnUnequip()
    self.Owner.Move = OriginalPlayerMoveFunc
end


return class