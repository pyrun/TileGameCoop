object = -1

function turn_end( id)
	setAnimation( id, "fly")
end

local dead_timer = 1

function start( id)
	setAnimation( id, "fly")
end


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
	l_rect_x = 200
	l_rect_y = 200
	ids = {findObjects( id, l_x-100, l_y-100, l_rect_x, l_rect_y)}

	position_x = l_x
	position_y = l_y
	table.sort( ids, compare)

	for object_id = 1, #ids do
		local obj = ids[object_id]
		l_obj_x, l_obj_y = getPosition( obj)

		local dir = getAnimationDirection( id)

		if isPlayer( obj) and isAlive( obj) then
			if dir == false and l_obj_x > l_x and getAnimation( id) == "fly" and isAlive( object) == false then
				setAnimationDirection( id, true)
				setAnimation( id, "turn")
			end
			if dir == true and l_obj_x < l_x and getAnimation( id) == "fly" and isAlive( object) == false then
				setAnimationDirection( id, false)
				setAnimation( id, "turn")
			end

			if getAnimation( id) == "fly" and isAlive( object) == false then
				target_angle = math.atan2  ( (l_obj_y+16) - l_y, l_obj_x - l_x)

				speed_x = math.cos( target_angle) * 0.08
				speed_y = math.sin( target_angle) * 0.08

				object = createObject( "fireball", l_x, l_y)
				setVelocityX( object, speed_x)
				setVelocityY( object, speed_y)
				--play_sound( id, "throw")
			end
		end
	end
end

function collision( id, ...)
	if isAlive( id) == false then
		do return end
	end
	for k,v in pairs({...}) do
		if isAlive( v) == true then
			if isPlayer( v) == true then		
				kill( v)
			end
		end
	end
end
