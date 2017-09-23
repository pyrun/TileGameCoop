local object = -1


function timer( id, time)
	if isAlive( id) == false then
		do return end
	end
	l_x, l_y = getPosition( id)
	if isAlive( object) == false then
		object = createObject( "stalactite", l_x + 8, l_y + 10)
		setVelocityY( object, 0.1)
	end
end
