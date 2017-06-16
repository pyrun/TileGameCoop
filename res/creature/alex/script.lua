--package.path = package.path .. ";creature/human/?.lua"
--require( "script")

local move_speed = 0.02

function vertexhit( id)
end

function attack( id)

end

function liquid( id, swim)

end

function update( id)
	l_velX, l_velY = getVelocity( id)
	
	if math.abs(l_velX) > 0.0 then
		local dir = false
		if l_velX < 0.00 then
			dir = true
		end
		setAnimationDirection( id, dir)
	end

	if math.abs(l_velX) > 0.01 then
		setAnimation( id, "walk_side")
	elseif l_velY < -0.01 then
		setAnimation( id, "walk_n")
	elseif l_velY > 0.01 then
		setAnimation( id, "walk_s")
	end


	if getAnimation( id) == "walk_n" then
		if math.abs(l_velY) < 0.01 then
			setAnimation( id, "idle_n")
		end
	end
	
	if getAnimation( id) == "walk_s" then
		if math.abs(l_velY) < 0.01 then
			setAnimation( id, "idle_s")
		end
	end
	
	if getAnimation( id) == "walk_side" then
		if math.abs(l_velX) < 0.01 then
			setAnimation( id, "idle_side")
		end
	end
end


function jump( id)

end

function up( id) 
	addVelocity( id, 0, -move_speed)
end

function down( id)
	addVelocity( id, 0, move_speed)
end

function right( id)
	addVelocity( id, move_speed, 0)
end

function left( id)
	addVelocity( id, -move_speed, 0)
end

function run( id, press)

end

