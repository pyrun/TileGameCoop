local jump_speed = 0.5

function vertexhit( id)
end

function start( id) 
	setAnimation( id, "close")
end

function timer( id) 
	if getAnimation( id) == "open" then
		setAnimation( id, "close")
	end
end

function collision( id, ...)
	if isAlive( id) == false then
		do return end
	end
	for k,v in pairs({...}) do
		if getAnimation( id) == "close" then
			setAnimation( id, "open")
			setVelocityY( v, -jump_speed)
		end
	end
end
