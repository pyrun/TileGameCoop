local text = {}

local table_text = {}

function text.table_start()
	table.insert( table_text, "Guten Morgen Neuling")
	table.insert( table_text, "Du bist Programmierer in einer")
	table.insert( table_text, "der bedeutsamsten Institutionen")
	table.insert( table_text, "der Technickbranche, in dieser")
	table.insert( table_text, "Zeit")
	table.insert( table_text, "ENTER")

	table.insert( table_text, "Technische Fortschritte")
	table.insert( table_text, "belebten die Zeit und")
	table.insert( table_text, "evolutionaere Errungenschaften")
	table.insert( table_text, "konnten erreicht werden, doch")
	table.insert( table_text, "die Technick ist noch laengst")
	table.insert( table_text, "ENTER")

	table.insert( table_text, "nicht ausgereift.")
	table.insert( table_text, "Dein Aufgabengebiet widmet sich")
	table.insert( table_text, "Portalen, die mit erhoehtem")
	table.insert( table_text, "Druck des Firmenvorstandes,")
	table.insert( table_text, "fertiggestellt werden sollen")
	table.insert( table_text, "ENTER")

	table.insert( table_text, "Nach einigen Versuchen erscheint")
	table.insert( table_text, "ein Portal...")
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
