function timer( id, time)

end

function collision( id, ...)
	if isAlive( id) == false then
		do return end
	end
	for k,v in pairs({...}) do
		if isAlive( v) == true then
			if isEnemy( v) == true then		
				kill( v)
			end
		end
	end
end
