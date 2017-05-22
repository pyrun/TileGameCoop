local load_level_flag = false

function timer( id, time)
	if load_level_flag == true then 
		setLoadLevel( global_value, true )
		kill ( id)
		load_level_flag = false
	end
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
				setLoadLevel( "champselect.tmx", false )
				load_level_flag = true
			end
			do return end
		else
			setLoadLevel( "champselect.tmx", false )
			load_level_flag = true
		end
	end
end
