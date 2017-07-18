function vertexhit( id)
	delete( id);
end

function collision( id, ...)
	for k,v in pairs({...}) do
		if isAlive( v) == true then
			if isPlayer( v) == true then
				kill( v)
			end
		end
	end
end
