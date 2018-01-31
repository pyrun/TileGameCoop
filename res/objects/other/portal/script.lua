local dead_timer = 2

function timer( id)
	if isAlive( id) == false then
		dead_timer = dead_timer - 1
		if dead_timer == 0 then
			l_x, l_y = getPosition( id)
			l_x = l_x - 10
			l_y = l_y - 10
			createObject( global_value, l_x, l_y)
			delete( id)
		end
		do return end
	end
end

function collision( id, ...)
	if isAlive( id) == false then
		do return end
	end
	for k,v in pairs({...}) do
		kill( id)
	end
end
