local float_speed = 0.007

local start_y = 0

function start( id) 
	if global_value == NIL then
		global_value = "100"
	end
	t, start_y = getPosition( id)
	start_y = start_y - 1
	addVelocity( id, 0, -0.05)
end

function timer( id, time)
	pos_x, pos_y = getPosition( id)

	if start_y-pos_y > tonumber(global_value) then
		addVelocity( id, 0, float_speed)
	end
	if start_y-pos_y < 0 then
		down = true
		addVelocity( id, 0, -float_speed)	
	end
		
end

