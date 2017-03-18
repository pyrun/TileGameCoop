function vertexhit( id)
end

local up_time = 100
local start_y = 0
local down = false

function start( id) 
	t, start_y = getPosition( id)
	start_y = start_y - 1
end

function timer( id, time)
	pos_x, pos_y = getPosition( id)

	if start_y-pos_y < up_time and down == false then
		setVelocityY( id, -0.05)
	else
		down = true
		setVelocityY( id, 0.05)	
	end
	
	if down == true and start_y-pos_y < 0 then
		down = false
	end
		
end

function collision( id, ...)

end
