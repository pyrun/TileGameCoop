local object = -1

function timer( id, time)
	if isAlive( id) == false then
		do return end
	end
	l_x, l_y = getPosition( id)
	if isAlive( object) == false then
		object = createObject( "fireball", l_x + 10, l_y + 10)
		setVelocityX( object, -0.1)
		play_sound( id, "throw")
	end
end
