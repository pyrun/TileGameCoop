function vertexhit( id)
end

function start( id) 
	global_value = "Test"
	global_value_2 = "1.0"
end

function timer( id, time) 
	message( id, tonumber( global_value_2), 0, 0, false, time+1, global_value)
end
