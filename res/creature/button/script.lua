function vertexhit( id)
end

function start( id) 

end

function timer( id) 

end

function signal( id, fromId, data)
	if data == "press" then
		if getAnimation( id ) == "idle" then
			setAnimation( id, "press")
			sendSignal( tonumber(global_value), id, "open")
		else
			setAnimation( id, "idle")
			sendSignal( tonumber(global_value), id, "close")
		end
	end
end