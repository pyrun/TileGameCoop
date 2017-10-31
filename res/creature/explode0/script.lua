local dead_timer = 13
local vel_x_speed = 0.1
local vel_y_speed = 0.35

function start( id)
	play_sound( id, "explode")
	kill(id)
end

function timer( id, time)
	dead_timer = dead_timer - 1
	if dead_timer == 0 then
		delete( id);
	end
end

done = false

function collision( id, ...)
	if done == true then
		do return end
	end
	for k,v in pairs({...}) do
		if v == id then
		else
			if getName( v) == "alex" or
				getName( v) == "jan" or
				getName( v) == "dani"
				then
				l_X, l_Y = getPosition( v)
				l_posX, l_posY = getPosition( id)
				l_velX = -vel_x_speed
				l_velY = -vel_y_speed

				if l_X-l_posX > 0 then
					l_velX = vel_x_speed
				end

				setVelocityX( v, l_velX)
				setVelocityY( v, l_velY)
			elseif isEnemy(v) == true or getName( v) == "up_stone" then
				kill( v)
			end
		end
	end
	done = true
end