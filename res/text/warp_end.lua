local text = {}

local table_text = {}

function text.table_start()
	table.insert( table_text, "Die Suche nach dem Ausgang")
	table.insert( table_text, "erstreckte sich nun ueber")
	table.insert( table_text, "mehrere Tage, unterschiedliche")
	table.insert( table_text, "Zeiten und abwechselnde Gebiete")
	table.insert( table_text, "und ein Ausweg war weiterhin")
	table.insert( table_text, "ENTER")

	table.insert( table_text, "nicht in Sicht. Ein Projekt")
	table.insert( table_text, "dieses Ausmassses hatte es")
	table.insert( table_text, "bisher noch nicht gegeben und")
	table.insert( table_text, "du wuerdest bis zum Schluss")
	table.insert( table_text, "dafuer kaempfen zurueck in die")
	table.insert( table_text, "ENTER")

	table.insert( table_text, "Realitaet zu gelangen.")
	table.insert( table_text, "Du betrittst das naechste Portal")
	table.insert( table_text, "und findest dich in deinem")
	table.insert( table_text, "Labor wieder, umstellt vom")
	table.insert( table_text, "Vorstand, der hochzufrieden die")
	table.insert( table_text, "ENTER")

	table.insert( table_text, "Technologie praesentiert")
	table.insert( table_text, "ENTER")

	table.insert( table_text, "Was fuer ein erster Arbeitstag.")
	table.insert( table_text, "END")
end

function text.get( id)
	return table_text[id]
end

function text.del( id)
	table.remove( table_text, id)
end

return text
