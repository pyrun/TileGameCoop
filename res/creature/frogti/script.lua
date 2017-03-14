function vertexhit( id)
	if getVertexHit( id, 1) then
		setAnimationDirection( id, 1)
	end
	if getVertexHit( id, 2) then
		setAnimationDirection( id, 0)
	end
	
	down1 = getVertexHit( id, 3)
	down2 = getVertexHit( id, 4)

	if down1 == false and down2 == true then
		setAnimationDirection( id, true)
	end
	if down1 == true and down2 == false then
		setAnimationDirection( id, false)
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
		setAnimation( id, "walk")
		if dir == 0 then
			addVelocity( id, -0.03, 0)
		else
			addVelocity( id, 0.03, 0)
		end
	end
end

function collision( id, ...)
	if isAlive( id) == false then
		do return end
	end
	for k,v in pairs({...}) do
		if isAlive( v) == true then
			if isenemy( v) == true then		
				kill( v)
			end
		end
	end
end
