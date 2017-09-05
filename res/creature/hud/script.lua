function start( id)
	global_value = "1"
	setPosition( id, 175, 50)
end

function timer( id, time)
	if getAmountPlayerActive() > 0 then

		player_number = tonumber(global_value)
	
		entity = getPlayerChamp( player_number )
		name = getName( entity)
		-- first letter upskin
		name = name:gsub("^%l", string.upper)
	
		message( id, 1, 6, 0, true, time+1, name)
	
		message( id, 0.9, 16, 14, true, time+1, "run")
		message( id, 0.9, 15, 26, true, time+1, "jump")
		
		if name == "Alex" then
			textx = "bomb"
			texta = "punch"
		elseif name == "Dani" then
			textx = "whip"
			texta = "throw"
		else
			textx = "shield"
			texta = "portal"
		end
		message( id, 0.9, 74, 13, true, time+1, textx)
		message( id, 0.9, 73, 26, true, time+1, texta)
	end
end
