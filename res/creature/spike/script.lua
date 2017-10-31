function timer( id)
	if isAlive( id) == false then
		do return end
	end
end

function used( id )
	l_x, l_y = getPosition( id)
	l_rect_x = 15
	l_rect_y = 16
	ids = {findObjects( id, l_x, l_y, l_rect_x, l_rect_y)}
	for object_id = 1, #ids do
		local obj = ids[object_id]
		if isEnemy(obj) == true or isPlayer( obj) == true then
			kill( obj)
		end
	end
end

function collision( id, ...)
	if isAlive( id) == false then
		do return end
	end

	for k,v in pairs({...}) do
		if isEnemy( v) == true or isPlayer( v) == true then
			setAnimation( id, "used")
			kill( v)
		end
	end
end
