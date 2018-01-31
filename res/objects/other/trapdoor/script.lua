function vertexhit( id)
end

function start( id) 
	if getAnimation(id) == "idle" then
		setSolid( id, true)
	else
		setSolid( id, false)
	end
end

function signal( id, fromId, data)
	if data == "close" then
		setSolid( id, true)
		setAnimation( id, "idle")
		play_sound( id, "close")
	end
	if data == "open" then
		setSolid( id, false)
		setAnimation( id, "open")
		play_sound( id, "open")
	end
end

function collision( id, ...)

end
