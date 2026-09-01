
local Graphics = Game.Graphics
local VertexArray = Graphics.VertexArray
local VertexBuffer = Graphics.VertexBuffer
local ElementBuffer = Graphics.ElementBuffer
local Program = Graphics.Program

local class = setmetatable({}, Assets.BaseClassesClient.ItemBase)

class.Name = "Dagger"
class.Description = "chuddy forgot to set the description"
class.BookEntry = "chuddy forgot to set the book description"

class.__index = class


class.GraphicsObjects = {
	DaggerVAO = VertexArray.new();
	DaggerVBO = VertexBuffer.new();
	DaggerEBO = ElementBuffer.new();

	DaggerProgram = Program.new();
}
do


end


function class.new()
	
	local new = {}
	setmetatable(new, class)

	return new
end


function class:UsePrimary()
	print("use primary")
end

function class:UseSecondary()
	print("use secondary")
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