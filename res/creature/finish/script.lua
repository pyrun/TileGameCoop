function timer( id, time)

end

function collision( id, ...)
	player = 0

	for k,v in pairs({...}) do
		if isAlive( v) == true and isPlayer( v) == true then	
			player = player + 1
		end
	end

	if player == 3 then
		setLoadLevel( global_value, true )
	end
end