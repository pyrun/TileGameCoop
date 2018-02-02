function vertexhit( id)
end

function start( id) 
	global_value = "debug"
	global_value_2 = "Debug - Text"
end

function timer( id) 
	message( id, 1.5, 18, 2, false, 1000, global_value_2)
end

function collision( id, ...)
	if isAlive( id) == false then
		do return end
	end
	-- special
	if global_value == "load" then
		loadstate()
	elseif global_value == "quit" then
		setConfig( "quit", "game", "true")
	elseif global_value == "fullscreen" then
		if getConfig( "fullscreen", "display") == "false" then
			setConfig( "fullscreen", "display", "true")
			setConfig( "display_change", "game", "true")
			setConfig( "display_change_mode", "game", "true")
		end
	else
		setLoadLevel( global_value, true )
	end
	kill( id)
end
