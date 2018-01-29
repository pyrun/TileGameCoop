function vertexhit( id)
end

function start( id) 
	setSolid( id, true)
	setAnimation( id, "close")
end

function open( id)
	play_sound( id, "open")
end

function close( id)
	play_sound( id, "close")
end

function signal( id, fromId, data)
	if data == "close" then
		setSolid( id, true)
		setAnimation( id, "close")
	end
	if data == "open" then
		setSolid( id, false)
		setAnimation( id, "idle")
	end
end

function collision( id, ...)

end
