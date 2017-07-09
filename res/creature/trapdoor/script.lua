function vertexhit( id)
end

function start( id) 
	setSolid( id, false)
	setAnimation( id, "close")
end

function timer( id) 
	if getAnimation( id) == "idle" then
		setSolid( id, false)
	elseif getAnimation( id) == "close" then
		setSolid( id, true)
	end
end

function collision( id, ...)

end
