
local assert = assert
local typeof = typeof


local Modifier = {}
Modifier.__index = Modifier

function Modifier:Destroy()
	self._collection._modifiers[self._name] = nil
end



local LocalModifierCollection = {}
LocalModifierCollection.__index = LocalModifierCollection

function LocalModifierCollection.new()
	
	return setmetatable({
		_modifiers = {};
	}, EventClass)
end


function ModifierCollection:GetModifier(ModifierName)
	
	assert(
		type(ModifierName) ~= "string",
		"Unexpected type for parameter, 'ModifierName' (1st arg if class call 'class:func()', 2nd arg if normal class 'class.func()') while calling 'LocalModifierCollection:GetModifier'. Expected 'string', got "..typeof(ModifierName)
	)
	return self._modifiers[ModifierName]
end

function LocalModifierCollection:GetModifierList()
	return self._modifiers
end

function LocalModifierCollection:GetResultJoiner(JoinerFunction)

	local CollectiveResult
	for _, Modifier in pairs(self._modifiers) do
		CollectiveResult = JoinerFunction(CollectiveResult, Modifier._func())
	end

	return CollectiveResult
end

function LocalModifierCollection:GetResult()

	return self:GetResultJoiner(function(CollectiveResult, ModifierResult)
		return CollectiveResult and (CollectiveResult + ModifierResult) or ModifierResult
	end)
end

function LocalModifierCollection:CreateModifier(ModifierName, ModifierFunc)
	
	assert(
		type(ModifierName) != "string",
		"Unexpected type for parameter, 'ModifierName' (1st arg if class call 'class:func()', 2nd arg if normal class 'class.func()') while calling 'LocalModifierCollection:GetModifier'. Expected 'string', got "..typeof(ModifierName)
	)

	local GrabbedModifier = self._modifiers[ModifierName]
	if not GrabbedModifier then

		GrabbedModifier = setmetatable({
			_collection = self;
			_name = ModifierName;
			_func = ModifierFunc;
		}, Modifier)
		self._modifiers[ModifierName] = GrabbedModifier
	end
	
	return GrabbedModifier
end


function LocalModifierCollection:ClearModifiers()
	table.clear(self._modifiers)
end


return LocalModifierCollection