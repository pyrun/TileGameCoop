local object = -1

function timer( id, time)
	if isAlive( id) == false then
		do return end
	end
	l_x, l_y = getPosition( id)

	l_y = l_y + 16;

	l_x = l_x + (math.random(28));

	object = createObject( "sharp_ice", l_x, l_y + 6)
	setVelocityY( object, 0.05)

	dir = math.random( 0, 1)

	if dir == 1 then
		setAnimationDirection( object, true)
	end
end
