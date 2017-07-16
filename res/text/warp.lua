local text = {}

local table_text = {}

function text.table_start()
	table.insert( table_text, "Du bist in einem Strudel ")
	table.insert( table_text, "gefangen und siehst auch deine ")
	table.insert( table_text, "Freunde  darin - die haben ")
	table.insert( table_text, "wohl dieses Portal als ")
	table.insert( table_text, "ENTER")
	table.insert( table_text, "erstes untersucht...")
	table.insert( table_text, "END")
end

function text.get( id)
	return table_text[id]
end

function text.del( id)
	table.remove( table_text, id)
end

return text
