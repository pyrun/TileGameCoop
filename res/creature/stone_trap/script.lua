local object = -1


function timer( id, time)
	if isAlive( id) == false then
		do return end
	end
	l_x, l_y = getPosition( id)

	l_x = l_x + (math.random(14));

	if isAlive( object) == false then
		object = createObject( "stalactite", l_x, l_y + 6)
		setVelocityY( object, 0.05)
	end
end
