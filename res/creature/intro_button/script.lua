function vertexhit( id)
end

function start( id) 
	global_value = "Test"
end

function timer( id, time)
	message( id, 1, 8, 5, global_value)
	if getAnimation( id) == "die" then
		if global_value == "Continue" then
			loadstate()
		else
			setLoadLevel( global_value_2, true )
		end
	end
end
