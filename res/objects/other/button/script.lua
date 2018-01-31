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
			for i in string.gmatch(global_value, "%S+") do
				sendSignal( tonumber(i), id, "open")
			end
		else
			setAnimation( id, "idle")
			for i in string.gmatch(global_value, "%S+") do
				sendSignal( tonumber(i), id, "close")
			end
		end
	end
end
