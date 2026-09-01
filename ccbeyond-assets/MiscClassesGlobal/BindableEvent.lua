
local ConnectionClass = {}
ConnectionClass.__index = ConnectionClass

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


local table_insert = table.insert
local assert = assert
local typeof = typeof
local typeid = typeid
function EventClass:Connect(Function)
	
	assert(typeid(Function) == 6, "Unexpected type while connecting to a BindableEvent. Expected 'function', got "..typeof(Function)) --`Unexpected type while connecting to a BindableEvent. Expected 'function', got {typeof(Function)}`)
	
	local new
	new = setmetatable({
		_connectionsTable = self._connections;
		_index = 0;
		_func = Function;
	}, ConnectionClass)
	new._index = table_insert(self._connections, new)
	return new
end


local table_clear = table.clear
function EventClass:ClearConnections()
	table_clear(self._connections)
end


local spawn = spawn
function EventClass:Fire(...)
	
	for _, Connection in ipairs(self._connections) do
		spawn(Connection._func, ...)
	end
end


return EventClass