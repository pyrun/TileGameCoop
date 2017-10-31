function vertexhit( id)
end

function start( id) 

end

time = 2

function timer( id) 
	time = time + 1
	if global_value_2 == "active" and time > 2 and getAnimation(id) == "press" then
		for i in string.gmatch(global_value, "%S+") do
			sendSignal( tonumber(i), id, "close")
		end
		setAnimation( id, "idle")
	end
end

function collision( id, ...)
	for k, obj in pairs({...}) do
		if isPlayer( obj) == true and not(getAnimation(id) == "press") then
			setAnimation( id, "press")
			for i in string.gmatch(global_value, "%S+") do
				if getAnimation( tonumber(i)) == "open" then
					sendSignal( tonumber(i), id, "close")
				else
					sendSignal( tonumber(i), id, "open")
				end
			end
		end
		if isPlayer( obj) == true then
			time = 0
		end
	end
end

function press( id)
	
end
