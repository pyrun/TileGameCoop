function vertexhit( id)
end

function start( id) 
	global_value = "debug"
	global_value_2 = "Debug - Text"
end

function timer( id) 
	config_set = getconfig( global_value)
	if config_set == true then
		setAnimation( id, "active")
	else
		setAnimation( id, "idle")
	end
	message( id, 0.9, 18, 2, global_value_2)
end

function collision( id, ...)
	if isAlive( id) == false then
		do return end
	end
	config_value = getconfig( global_value)
	setconfig( global_value, not config_value)
	kill( id)
	end_level( true)
end
