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
	l_rect_x = 31
	l_rect_y = 100
	ids = {findObjects( id, l_x, l_y-100, l_rect_x, l_rect_y)}
	local found = false

	for object_id = 1, #ids do
		local obj = ids[object_id]

		if isPlayer( obj) and isAlive( obj) then
			found = true
		end
	end

	if found == true then
		setVelocityY( id, -0.3)
		play_sound( id, "up")
	end
end

function collision( id, ...)
	for k,v in pairs({...}) do
		if isAlive( v) == true then
			if isPlayer( v) == true then
				kill( v)
			end
		end
	end
end
