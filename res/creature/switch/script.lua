function vertexhit( id)
end

function start( id) 

end

function timer( id) 

end

function turn_end( id)
	setAnimation( id, "press")
	play_sound( id, "press")
	for i in string.gmatch(global_value, "%S+") do
		sendSignal( tonumber(i), id, "open")
	end
end

function turn2_end( id)
	setAnimation( id, "idle")
	play_sound( id, "unpress")
	for i in string.gmatch(global_value, "%S+") do
		sendSignal( tonumber(i), id, "close")
	end
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