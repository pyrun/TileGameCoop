local text = {}

local table_text = {}

function text.table_start()
	table.insert( table_text, "Du arbeitest als Programmierer")
	table.insert( table_text, "in deiner Lab als dir im neben-")
	table.insert( table_text, "zimmer ein Portal auffiel.")
	table.insert( table_text, "ENTER")
	table.insert( table_text, "Du hast deine Arbeit")
	table.insert( table_text, "fortgeführt bis dir")
	table.insert( table_text, "ein Bug in deinem")
	table.insert( table_text, "Programm augefallen ist")
	table.insert( table_text, "ENTER")
	table.insert( table_text, "Scheibenkleister!")
	table.insert( table_text, "END")
end

function text.get( id)
	return table_text[id]
end

function text.del( id)
	table.remove( table_text, id)
end

return text
