function timer( id)
	if isAlive( id) == false then
		do return end
	end
end

function collision( id, ...)
	if isAlive( id) == false then
		do return end
	end
	for k,v in pairs({...}) do
		if isAlive( v) == true and isPlayer( v) == true then
			kill( v)
		end
	end
end
