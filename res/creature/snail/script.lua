function vertexhit( id)
	if getVertexHit( id, 1) then
		setAnimationDirection( id, 1)
	end
	if getVertexHit( id, 2) then
		setAnimationDirection( id, 0)
	end
end

function timer( id, time)
	local dir = getAnimationDirection( id)
	setAnimation( id, "walk")
	if dir == 0 then
		addVelocity( id, -0.03, 0)
	else
		addVelocity( id, 0.03, 0)
	end
end