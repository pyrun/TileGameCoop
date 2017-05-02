function timer( id, time)
	if howManyPlayerEntity() <= 0 then
		l_x, l_y = getPosition( id)
		createObject( global_value, l_x, l_y)
	end
	delete( id)
end
