function vertexhit( id)
end

function start( id) 
end

function timer( id) 
end

function collision( id, ...)
	if isAlive( id) == false then
		do return end
	end

	setLoadLevel( global_value, true )
	kill( id)
end
