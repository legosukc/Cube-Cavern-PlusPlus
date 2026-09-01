
local Items = {
	Items = {}
}


function Items:Spawn(ItemClass)
	
	local NewItem = ItemClass.new()
	table.insert(self.Items, NewItem)
	return NewItem
end


function Items:Update()
	
	for _, Item in ipairs(self.Items) do
		
		if Item.Update then
			Item:Update()
		end
	end
end

function Items:Draw()
	
	for _, Item in ipairs(self.Items) do
		
		if Item.Draw then
			Item:Draw()
		end
	end
end


return Items