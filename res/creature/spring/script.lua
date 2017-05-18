local jump_speed = 0.5

function vertexhit( id)
end

function start( id) 
	setAnimation( id, "close")
end

function timer( id) 
	if getAnimation( id) == "open" then
		setAnimation( id, "close")
	end
end

function collision( id, ...)
	if isAlive( id) == false then
		do return end
	end
	if getAnimation( id) == "close" and getFrame( id) == 3 then
		l_x, l_y = getPosition( id)
		l_rect_x = 16
		l_rect_y = 16
		ids = {findObjects( id, l_x, l_y, l_rect_x, l_rect_y)}
		for object_id = 1, #ids do
			setVelocityY( ids[object_id], -jump_speed)
		end
		setAnimation( id, "open")
	end
end
