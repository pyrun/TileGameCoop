function vertexhit( id)
end

function start( id) 

end

time = 2

function timer( id) 
	time = time + 1
	if global_value_2 == "active" and time > 2 then
		setAnimation( tonumber(global_value), "close")
		setAnimation( id, "idle")
	end
end

function collision( id, ...)
	for k, obj in pairs({...}) do
		if isPlayer( obj) == true then
			setAnimation( id, "press")
			time = 0
		end
	end
end

function press( id)
	setAnimation( tonumber(global_value), "idle")
end
