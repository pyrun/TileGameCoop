function timer( id, time)
end

function getDistance(objAX, objAY, objBX, objBY)
    -- Get the length for each of the components x and y
    local xDist = objBX - objAX
    local yDist = objBY - objAY

    return math.sqrt( (xDist ^ 2) + (yDist ^ 2) ) 
end

function collision( id, ...)
	if isAlive( id) == false then
		do return end
	end
	for k,v in pairs({...}) do
		if getAnimation( v) == "join" then
			setLoadLevel( global_value, true )
			kill ( id)
		end
		local x, y = getPosition( v)
		local velx, vely = getVelocity( v)
		local myX, myY = getPosition( id)
		if math.abs(velx) > 0.01 or math.abs(vely) > 0.01 then
			if getDistance( x, y, myX, myY) > getDistance( x + velx, y + vely, myX, myY) then
				setVelocityX( v, 0)
				setVelocityY( v, 0)
			end
		end
	end
end
