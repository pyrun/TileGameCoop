speed = 0.03
turn_flag = true

function vertexhit( id)
	if isAlive( id) == false then
		do return end
	end

	local dir = getAnimationDirection( id)

	if turn_flag == true then
		if getVertexHit( id, 1) and dir == false then
			setAnimationDirection( id, true)
			setAnimation( id, "turn")
			turn_flag = false
		elseif getVertexHit( id, 2) and dir == true then
			setAnimationDirection( id, false)
			setAnimation( id, "turn")
			turn_flag = false
		end
	end
end

function turn_end( id)
	local dir = getAnimationDirection( id)
	
	setAnimation( id, "fly")
	turn_flag = true
end

local dead_timer = 1

function start( id)
	setAnimation( id, "fly")
end

function timer( id)
	if isAlive( id) == false then
		setGravity( id, true) 
		do return end
	end

	if getAnimation( id) == "idle" or getAnimation( id) == "fly" then

		local dir = getAnimationDirection( id)
		local velX, velY = getVelocity( id) 
		
		if dir == false and math.abs(velX) < speed then
			addVelocity( id, -0.01, 0)
		elseif math.abs(velX) < speed then
			addVelocity( id, 0.01, 0)
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
