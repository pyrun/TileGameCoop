local float_speed = 0.007

local start_y = 0

function start( id) 
	if global_value == NIL then
		global_value = "100"
	end

	t, start_y = getPosition( id)
	start_y = start_y - 1
end

local velocity = false

function timer( id, time)
	pos_x, pos_y = getPosition( id)
	velx, vely = getVelocity( id)

	if getAnimation( id) == "right" then
		if pos_x-start_y > tonumber(global_value) then
			addVelocity( id, -float_speed, 0)
		end
		if pos_x-start_y < 0 then
			down = true
			addVelocity( id, float_speed, 0)	
		end
		if velx < 0.1 and velx > -0.1 and velocity == false then
			addVelocity( id, 0.05, 0)
			velocity = true
			start_y, t = getPosition( id)
			start_y = start_y - 1
		end
	else

		if start_y-pos_y > tonumber(global_value) then
			addVelocity( id, 0, float_speed)
		end
		if start_y-pos_y < 0 then
			down = true
			addVelocity( id, 0, -float_speed)	
		end
		if vely < 0.1 and vely > -0.1 and velocity == false then
			addVelocity( id, 0, 0.05)
			velocity = true
		end
	end
		
end

