local jump_speed = 0.5

function start( id) 
	setAnimation( id, "close")
end

function EndCallOpen( id) 
	setAnimation( id, "close")
end

function EndCall( id) 
	setAnimation( id, "closed")
end

function collision( id, ...)
	if getAnimation( id) == "closed" then
		l_x, l_y = getPosition( id)
		l_rect_x = 16
		l_rect_y = 16
		local found = false
		ids = {findObjects( id, l_x, l_y, l_rect_x, l_rect_y)}
		for object_id = 1, #ids do
			found = true
			setVelocityY( ids[object_id], -jump_speed)
		end
		if found == true then
			setAnimation( id, "open")
		end
	end
end
