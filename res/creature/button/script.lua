function vertexhit( id)
end

function start( id) 

end

function timer( id) 

end

function collision( id, ...)
	if isAlive( id) == false or getAnimation( id) == "press" then
		do return end
	end
	for k,v in pairs({...}) do
		if getAnimation( tonumber(global_value)) == "close" then
			setAnimation( tonumber(global_value), "idle")
			setAnimation( id, "press")
		end
	end
end
