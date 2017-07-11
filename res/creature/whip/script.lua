l_offset_x = 10
l_offset_y = 13
l_rect_x = 16
l_rect_y = 3

function whip_end( id)
	l_x, l_y = getPosition( id)
	ids = {findObjects( id, l_x + l_offset_x, l_y + l_offset_y, l_rect_x, l_rect_y)}
	for object_id = 1, #ids do
		local obj = ids[object_id]
		if isEnemy(obj) == true then
			kill( obj)
		end
	end
	delete( id)
end
