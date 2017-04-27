function timer( id, time)
end

function getDistance(objAX, objAY, objBX, objBY)
    -- Get the length for each of the components x and y
    local xDist = objBX - objAX
    local yDist = objBY - objAY

    return math.sqrt( (xDist ^ 2) + (yDist ^ 2) ) 
end

function collision( id, ...)
	for k,v in pairs({...}) do
		
		if isAlive( id) == false then
			if getAnimation( v) == "join" then
				setLoadLevel( global_value, true )
				--kill ( id)
			end
			do return end
		else
			setLoadLevel( global_value, true )
			kill ( id)	
		end
	end
end
