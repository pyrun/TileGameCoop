function vertexhit( id)
end

function start( id) 

end

function timer( id) 

end

function turn_end( id)
	setAnimation( id, "press")
	sendSignal( tonumber(global_value), id, "open")
end

function turn2_end( id)
	setAnimation( id, "idle")
	sendSignal( tonumber(global_value), id, "close")
end

function signal( id, fromId, data)
	if data == "blow" then
		if getAnimation( id ) == "idle" then
			setAnimation( id, "turn")
		elseif getAnimation( id ) == "press" then
			setAnimation( id, "turn2")
		end
	end
end