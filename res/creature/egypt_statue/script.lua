local object = -1

local position_x, position_y

function compare(a,b)
	l_compare_x, l_compare_y = getPosition( a)
	l_obj_compare_x, l_obj_compare_y = getPosition( b)
	return distance( position_x, position_y, l_compare_x, l_compare_y) < distance( position_x, position_y, l_obj_compare_x, l_obj_compare_y)
end

function distance( x1, y1, x2, y2 )
	return math.sqrt( (x2-x1)^2 + (y2-y1)^2 )
end

function timer( id)
	if isAlive( id) == false then
		setGravity( id, true) 
		do return end
	end

	l_x, l_y = getPosition( id)
	l_x = l_x
	l_y = l_y
	l_rect_x = 400
	l_rect_y = 400
	ids = {findObjects( id, l_x-200, l_y-200, l_rect_x, l_rect_y)}

	position_x = l_x
	position_y = l_y
	table.sort( ids, compare)

	for object_id = 1, #ids do
		local obj = ids[object_id]
		l_obj_x, l_obj_y = getPosition( obj)

		local dir = getAnimationDirection( id)

		if isPlayer( obj) and isAlive( obj) then
			
			target_angle = math.atan2  ( (l_obj_y+16) - l_y, l_obj_x - l_x)

			speed_x = math.cos( target_angle) * 0.08
			speed_y = math.sin( target_angle) * 0.08

			object = createObject( "fireball", l_x+5, l_y+5)
			setVelocityX( object, speed_x)
			setVelocityY( object, speed_y)
			play_sound( id, "throw")
		end
	end
end