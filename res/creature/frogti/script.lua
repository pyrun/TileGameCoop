function vertexhit( id)
	if getVertexHit( id, 1) then
		setAnimationDirection( id, 1)
	end
	if getVertexHit( id, 2) then
		setAnimationDirection( id, 0)
	end
end

local dead_timer = 1

function timer( id, time)
	if isAlive( id) == false then
--		dead_timer = dead_timer - 1
--		if dead_timer == 0 then
--			l_x, l_y = getPosition( id)
--			createObject( "explode1", l_x, l_y)
--			delete( id);
--		end
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
		if isAlive( v) == true and isenemy( v) == true then		
			kill( v)
		end
	end
end
