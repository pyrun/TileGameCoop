function vertexhit( id)
end

function start( id) 

end

time = 2

function timer( id) 
	time = time + 1
	if global_value_2 == "active" and time > 2 and getAnimation(id) == "press" then
		sendSignal( tonumber(global_value), id, "close")
		setAnimation( id, "idle")
	end
end

function collision( id, ...)
	for k, obj in pairs({...}) do
		if isPlayer( obj) == true and not(getAnimation(id) == "press") then
			setAnimation( id, "press")
			sendSignal( tonumber(global_value), id, "open")
		end
		if isPlayer( obj) == true then
			time = 0
		end
	end
end

function press( id)
	
end
