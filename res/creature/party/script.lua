counter = 0

function timer( id, time)
	counter = counter + 1

	if counter > 5 then
		setLoadLevel( global_value, true )
	end
end