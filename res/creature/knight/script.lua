function vertexhit( id)
end

function update( id)
	local l_velX, l_velY
    	-- get velocity
	l_velX, l_velY = getVelocity( id)

	if l_velY < 0.0 then
		setAnimation( id, "jump")
	end
	
	if getColision( id, "down") then
		if math.abs(l_velX) > 0.01 then
			setAnimation( id, "walk")
		end
		if math.abs(l_velX) < 0.01 then
			setAnimation( id, "idle")
		end
	end

	if math.abs(l_velX) > 0.0 then
		local dir = 0
		if l_velX < 0.00 then
			dir = 1
		end
		setAnimationDirection( id, dir)
	end
end

local jump_two = 0
local jump_high = -0.32

function jump( id)
	if getColision( id, "down") then
		addVelocity( id, 0, jump_high)
		jump_two = 1
    	elseif jump_two == 1 then
        jump_two = 0
        setVelocityY( id, jump_high/2)
	end
end

function up( id) 
	io.write("up\n")
end

function down( id) 
	io.write("down\n")
end

local max_speed = 0.1
local walk_speed = 0.03

function right( id)
	local l_velX, l_velY
    	-- get velocity
	l_velX, l_velY = getVelocity( id)
	if getColision( id, "down") then
       		if l_velX < max_speed then
           		 addVelocity( id, walk_speed, 0 )
       		end
    	else
        	if l_velX < max_speed then
            		addVelocity( id, walk_speed, 0 )
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
        	if l_velX > -max_speed then
            		addVelocity( id, -walk_speed, 0 )
        	end
	end
end

function run( id, press)
	if press then
		max_speed = 0.2
	else
		max_speed = 0.1
	end
end

