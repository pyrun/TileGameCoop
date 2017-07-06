local text = {}

local table_text = {}

function text.table_start()
	table.insert( table_text, "Once a programmer wrote a")
	table.insert( table_text, "simple app and insert a copy")
	table.insert( table_text, "of his brain. He didnt know")
	table.insert( table_text, "what he does but it works")
	table.insert( table_text, "ENTER")
	table.insert( table_text, "This release LUA the KI")
	table.insert( table_text, "SHE or IT helps the humans")
	table.insert( table_text, "but the military dont like her")
	table.insert( table_text, "and wanna kill the ki with")
	table.insert( table_text, "ENTER")
	table.insert( table_text, "the owner.")
	table.insert( table_text, "END")
end

function text.get( id)
	return table_text[id]
end

function text.del( id)
	table.remove( table_text, id)
end

return text
