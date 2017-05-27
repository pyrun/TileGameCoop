function start( id)
	if global_value == NIL then
		global_value = "alex jan"
	end
end

function timer( id, time)
	if howManyPlayerEntity() <= 0 then
		l_x, l_y = getPosition( id)
		local shift = 0
		for w in global_value:gmatch("%S+") do
			createObject( w, l_x + shift, l_y)
			shift = shift + 16
		end
	end
	delete( id)
end
