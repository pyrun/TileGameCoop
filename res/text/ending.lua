local text = {}

local table_text = {}

function text.table_start()
	table.insert( table_text, "Vielen Dank fuer das Spielen!")
	table.insert( table_text, "Bitte schaut euch auch die")
	table.insert( table_text, "Creddits an.")
	table.insert( table_text, "Have a nice day!")
	table.insert( table_text, "ENTER")
	table.insert( table_text, "PS:")
	table.insert( table_text, "Ihr/du seit ingesammt " .. getPlayerData( "death") .. "")
	table.insert( table_text, "gestorben.")
	table.insert( table_text, "Ciau!")
	table.insert( table_text, "END")
end

function text.get( id)
	return table_text[id]
end

function text.del( id)
	table.remove( table_text, id)
end

return text
