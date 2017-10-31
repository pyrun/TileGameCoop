local text = {}

local table_text = {}

function text.table_start()
	table.insert( table_text, "Guten Morgen Neuling")
	table.insert( table_text, "Du bist Programmierer in einer")
	table.insert( table_text, "der bedeutsamsten Institutionen")
	table.insert( table_text, "der Welt, wenn es um die")
	table.insert( table_text, "Technologie mit Portalen geht.")
	table.insert( table_text, "ENTER")

	table.insert( table_text, "Das Projekt weisst jedoch ")
	table.insert( table_text, "erhebliche Bugs auf. Der")
	table.insert( table_text, "Druck von den Vorgesetzten ist")
	table.insert( table_text, "gross und erleichtert keines")
	table.insert( table_text, "Wegs die Arbeit, sondern")
	table.insert( table_text, "ENTER")
	table.insert( table_text, "provoziert nur weitere Fehler.")
	table.insert( table_text, "END")

	-- set config player
	setPlayerData( "death", tostring(0));
end

function text.get( id)
	return table_text[id]
end

function text.del( id)
	table.remove( table_text, id)
end

return text
