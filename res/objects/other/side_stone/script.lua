local hit_down = false

function vertexhit( id)
	if getColision( id, "down") then
		--set flag
		hit_down = true
	end
end

function timer( id )
	if isAlive( id) == false then
		do return end
	end

	l_x, l_y = getPosition( id)
	l_rect_x = 31+60
	l_rect_y = 31
	ids = {findObjects( id, l_x, l_y, l_rect_x, l_rect_y)}
	local found = false

	for object_id = 1, #ids do
		local obj = ids[object_id]

		if isPlayer( obj) and isAlive( obj) then
			found = true
		end
	end

	if found == true then
		setVelocityX( id, 0.03)
		setVelocityY( id, -0.3)
	end

	l_x, l_y = getPosition( id)
	l_rect_x = 31+60
	l_rect_y = 31
	ids = {findObjects( id, l_x-60, l_y, l_rect_x, l_rect_y)}
	found = false

	for object_id = 1, #ids do
		local obj = ids[object_id]

		if isPlayer( obj) and isAlive( obj) then
			found = true
		end
	end

	if found == true then
		setVelocityX( id, -0.03)
		setVelocityY( id, -0.3)
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
