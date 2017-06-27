local text = {}

local table_text = {}

function text.table_start()
	table.insert( table_text, "Template")
	table.insert( table_text, "Test1")
	table.insert( table_text, "Hello World")
	table.insert( table_text, "Hello World2")
	table.insert( table_text, "Hello World3")
	table.insert( table_text, "ENTER")
	table.insert( table_text, "Side 2")
	table.insert( table_text, "END")
end

function text.get( id)
	return table_text[id]
end

function text.del( id)
	table.remove( table_text, id)
end

return text
