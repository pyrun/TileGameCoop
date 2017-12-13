local text = {}

local table_text = {}

function text.table_start()
	table.insert( table_text, "Continue...")
	table.insert( table_text, "Thanks for Playing!")
	table.insert( table_text, "You died " .. getPlayerData( "death") .. " times")
	table.insert( table_text, "END")
end

function text.get( id)
	return table_text[id]
end

function text.del( id)
	table.remove( table_text, id)
end

return text
