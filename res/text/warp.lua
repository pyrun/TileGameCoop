local text = {}

local table_text = {}

function text.table_start()
	table.insert( table_text, "Ein starker Sog zieht deine")
	table.insert( table_text, "Kollegen ploetzlich in ein")
	table.insert( table_text, "nicht kontrollierbares Portal ")
	table.insert( table_text, "und ehe du dich dagegen wehren ")
	table.insert( table_text, "kannst bist du auch mittendrin..")
	table.insert( table_text, "END")
end

function text.get( id)
	return table_text[id]
end

function text.del( id)
	table.remove( table_text, id)
end

return text
