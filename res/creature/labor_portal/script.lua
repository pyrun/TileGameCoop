function vertexhit( id)
end

function start( id) 
end

function timer( id) 
	l_x, l_y = getPosition( id)
	l_x = l_x
	l_y = l_y
	l_rect_x = 200
	l_rect_y = 200
	ids = {findObjects( id, l_x-100, l_y-100, l_rect_x, l_rect_y)}

	position_x = l_x
	position_y = l_y

	for object_id = 1, #ids do
		local obj = ids[object_id]
		l_obj_x, l_obj_y = getPosition( obj)

		local dir = getAnimationDirection( id)

		-- get alive
		if isAlive( obj) and getName( obj) == "alex" then
			target_angle = math.atan2  ( (l_y+16) - l_obj_y, l_x - l_obj_x)

			speed_x = math.cos( target_angle) * 0.08
			speed_y = math.sin( target_angle) * 0.08

			setVelocityX( obj, speed_x)
			setVelocityY( obj, speed_y)
		end
	end
end

function collision( id, ...)
	if isAlive( id) == false then
		do return end
	end

	setLoadLevel( global_value, true )
	kill( id)
end
