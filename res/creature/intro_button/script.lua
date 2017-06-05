function vertexhit( id)
end

function start( id) 
	global_value = "Test"
end

function timer( id, time)
	if getAnimation( id) == "die" then
		setLoadLevel( global_value_2, true )
	end

	message( id, 1, 8, 5, global_value)
end
