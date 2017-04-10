function vertexhit( id)
end

function start( id) 
	setSolid( id, false)
end

function timer( id) 
	if getAnimation( id) == "idle" then
		setSolid( id, true)
		setAnimation( id, "close")
--elseif getAnimation( id) == "close" then
--		setAnimation( id, "idle")
--		setSolid( id, false)
	end
end

function collision( id, ...)

end
