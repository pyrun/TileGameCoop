function vertexhit( id)
	if getVertexHit( id, 1) then
		setAnimationDirection( id, true)
	end
	if getVertexHit( id, 2) then
		setAnimationDirection( id, false)
	end
	if getColision( id, "down") then
		if getAnimation( id) == "jump" then
			setAnimation( id, "walk")
		end
	end
end

local dead_timer = 1

function timer( id)
	if isAlive( id) == false then
		do return end
	end

	if getColision( id, "down") and false then
		setVelocityY( id, -0.32)
		setAnimation( id, "jump")
	end
	
	if getColision( id, "down") then
		local dir = getAnimationDirection( id)
		if dir == false then
			addVelocity( id, -0.06, -0.25)
			setAnimation( id, "jump")
		else
			addVelocity( id, 0.06, -0.25)
			setAnimation( id, "jump")
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
