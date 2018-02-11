local object = -1


function timer( id, time)
	l_x, l_y = getPosition( id)
	if isAlive( object) == false then
		object = createObject( global_value, l_x, l_y)
		if not object == -1 then
			l_hitbox_x, l_hitbox_y = getHitbox( object)
			l_hitboxOff_x, l_hitboxOff_y = getHitboxOffset( object)
			setPosition( object, l_x+(l_hitbox_x/2), l_y+(l_hitbox_y/2))
		end
	end
end
