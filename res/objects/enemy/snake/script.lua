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


function start( id) 
	setAnimation( id, "walk")
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
				addVelocity( id, 0.016, 0)
			else
				addVelocity( id, -0.016, 0)
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
end

function collision( id, ...)
	if isAlive( id) == false then
		do return end
	end
	for k,v in pairs({...}) do
		if isAlive( v) == true and isPlayer( v) == true then
			kill( v)
		end
	end
end
