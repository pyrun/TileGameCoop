function vertexhit( id)
end

function start( id) 
	setAnimation( id, "close")
end

function timer( id) 
	if getAnimation( id) == "close" then
		setAnimation( id, "open")
	elseif getAnimation( id) == "open" then
		setAnimation( id, "close")
	end
end

function collision( id, ...)

end
