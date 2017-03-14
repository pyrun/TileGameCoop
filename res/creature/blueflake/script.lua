function vertexhit( id)
	--delete( id);
	if getVertexHit( id, 1) then
		delete( id);
	end
end

function collision( id, ...)
	for k,v in pairs({...}) do
		if isAlive( v) == true then
			if isenemy( v) == true then
				kill( v);
			end
		end
	end
end
