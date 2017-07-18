local object = -1


function timer( id, time)
	l_x, l_y = getPosition( id)
	if isAlive( object) == false then
		object = createObject( global_value, l_x, l_y)
	end
end
