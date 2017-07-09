function vertexhit( id)
	if getVertexHit( id, 1) then
		setAnimationDirection( id, false)
	end
	if getVertexHit( id, 2) then
		setAnimationDirection( id, true)
	end


	down1 = getVertexHit( id, 3)
	down2 = getVertexHit( id, 4)

	if down1 == false and down2 == true then
		setAnimationDirection( id, false)
	end
	if down1 == true and down2 == false then
		setAnimationDirection( id, true)
	end
end

local fly_y = 0
local dead_timer = 1
local start_x = 0
local float_speed = 0.005
local float_maxspeed = 0.05

function start( id) 
	addVelocity( id, 0, 0.003)

	if global_value == NIL then
		global_value = "200"
	end
	
	setAnimation( id, "walk")
	
	start_x, y = getPosition( id)
end

local fly_right

function timer( id, time)
	if isAlive( id) == false then
		do return end
	end

	pos_x, pos_y = getPosition( id)
	velx, vely = getVelocity( id)

	if getAnimation( id) == "walk" or getAnimation( id) == "idle" then
		if getGravity( id) == false then
			setGravity( id, true)
		end

		if getColision( id, "down") then
			local dir = getAnimationDirection( id)
			setAnimation( id, "walk")
			if dir == false then
				addVelocity( id, 0.008, 0)
			else
				addVelocity( id, -0.008, 0)
			end
		end
	end

	-- richtung anzeigen
	if math.abs(velx) > 0.0 then
		local dir = false
		if velx < 0.00 then
			dir = true
		end
		setAnimationDirection( id, dir)
	end

	if getAnimation( id) == "fly" then
		temp_x, temp_y = getPosition( id)
		if fly_y == 0 then
			fly_y = temp_y
		end

		if getGravity( id) == true then
			setGravity( id, false)
		end

		if temp_y-2 > fly_y and vely > -0.008 then
			addVelocity( id, 0, -0.008)
		end
		if temp_y+2 < fly_y and vely < 0.008 then
			addVelocity( id, 0, 0.008)
		end

		if fly_right == true then
			if velx > -float_maxspeed then
				addVelocity( id, -float_speed, 0)
			end
		else
			if velx < float_maxspeed then
				addVelocity( id, float_speed, 0)
			end
		end
		
		if pos_x-start_x > tonumber(global_value) then
			fly_right = true
		end
		if pos_x-start_x < 0  then
			fly_right = false
		end
		do return end
	end

	--if getGravity( id) == false then
	--	setGravity( id, true)
	--end
end

function collision( id, ...)
	for k,v in pairs({...}) do
		if isAlive( v) == true and isPlayer( v) == true then
			kill( v)
		end
	end
end
