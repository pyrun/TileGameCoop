function vertexhit( id)
	if getVertexHit( id, 1) then
		setAnimationDirection( id, true)
	end
	if getVertexHit( id, 2) then
		setAnimationDirection( id, false)
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

function update( id)
	if isAlive( id) == false then
		if getFrame( id) == 3 then
			l_x, l_y = getPosition( id)
			createObject( "explode1", l_x, l_y)
			delete( id)
		end
		do return end
	end
end

function timer( id, time)
	if isAlive( id) == false then
		do return end
	end

	local dir = getAnimationDirection( id)
	setAnimation( id, "walk")
	if dir == false then
		addVelocity( id, -0.03, 0)
	else
		addVelocity( id, 0.03, 0)
	end
end

function collision( id, ...)
	for k,v in pairs({...}) do
		if isAlive( v) == true and isPlayer( v) == true then
			kill( id)
		end
	end
end
