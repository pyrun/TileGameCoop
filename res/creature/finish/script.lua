function timer( id, time)

end

function collision( id, ...)
	for k,v in pairs({...}) do
		if isAlive( v) == true then	
			end_level( true)
		end
	end
end
