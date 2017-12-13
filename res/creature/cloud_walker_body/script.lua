function collision( id, ...)
	for k,v in pairs({...}) do
		if isAlive( v) == true and isPlayer( v) == true then
			kill( v)
		end
	end
end
