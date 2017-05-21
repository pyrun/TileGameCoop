local jump_speed = 0.5

function start( id) 
	setAnimation( id, "close")
end

function EndCallOpen( id) 
	setAnimation( id, "close")
end

function EndCall( id)
	l_x, l_y = getPosition( id)
	l_rect_x = 16
	l_rect_y = 16
	ids = {findObjects( id, l_x, l_y, l_rect_x, l_rect_y)}
	for object_id = 1, #ids do
		setVelocityY( ids[object_id], -jump_speed)
	end
	setAnimation( id, "open")
end
