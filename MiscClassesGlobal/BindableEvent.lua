
local ConnectionClass = {}
ConnectionClass.__index = ConnectionClass


local table_insert = table.insert
local table_clear = table.clear

local assert = assert
local typeof = typeof
local typeid = typeid

local setmetatable = setmetatable

local spawn = spawn


local function newConnection(Connections, Function)
	
	local new = {
		_connectionsTable = Connections;
		_index = 0;
		_func = Function;
	}
	new._index = table_insert(Connections, new)
	return setmetatable(new, ConnectionClass)
end

function ConnectionClass:Disconnect()
	
	local Connections = self._connectionsTable
	if Connections then

		Connections[self._index] = nil
		self._connectionsTable = nil
		self._index = nil
		self._func = nil
	end
end




local EventClass = {}
EventClass.__index = EventClass

function EventClass.new()
	
	return setmetatable({
		_connections = {};
	}, EventClass)
end


function EventClass:Connect(Function)
	
	assert(typeid(Function) == 6, "Unexpected type while connecting to a BindableEvent. Expected 'function', got "..typeof(Function)) --`Unexpected type while connecting to a BindableEvent. Expected 'function', got {typeof(Function)}`)
	
	return newConnection(self._connections, Function)
end

function EventClass:ClearConnections()
	table_clear(self._connections)
end

function EventClass:Fire(...)
	
	for _, Connection in ipairs(self._connections) do
		spawn(Connection._func, ...)
	end
end


return EventClass