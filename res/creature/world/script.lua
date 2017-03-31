function timer( id, time)
end

function collision( id, ...)
	if isAlive( id) == false then
		do return end
	end
	for k,v in pairs({...}) do
		setLoadLevel( global_value, true )
		kill ( id)
	end
end
