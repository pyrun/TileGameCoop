local jump_two = 0
local jump_high = -0.23

function jump( id)
	
	if getColision( id, "down") then
		addVelocity( id, 0, jump_high)
		jump_two = 1
    	elseif jump_two == 1 then
        jump_two = 0
        setVelocityY( id, jump_high/2)
	end
end

local max_speed = 0.5
local walk_speed = 0.1

function right( id)
    local l_velX, l_velY
    -- get velocity
	l_velX, l_velY = getVelocity( id)
	if getColision( id, "down") then
        if l_velX < max_speed then
            addVelocity( id, walk_speed, 0 )
        end
    else
        if l_velX < max_speed/4 then
            addVelocity( id, walk_speed/10, 0 )
        end
	end
end

function left( id)
    local l_velX, l_velY
    -- get velocity
	l_velX, l_velY = getVelocity( id)
	if getColision( id, "down") then
        if l_velX > -max_speed then
            addVelocity( id, -walk_speed, 0 )
        end
	else
        if l_velX > -max_speed/4 then
            addVelocity( id, -walk_speed/10, 0 )
        end
	end
end

io.write(string.format("Hello from %s\n", _VERSION))
