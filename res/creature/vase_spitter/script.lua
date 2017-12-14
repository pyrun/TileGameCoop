function timer( id)
	if isAlive( id) == false then
		do return end
	end
end

function pup( id )
	-- reset
	if getAnimation( id) == "idle" then
		setAnimation( id, "idle2")
	else
		setAnimation( id, "idle")
	end

	l_x, l_y = getPosition( id)

	for i=1,3 do
		object = createObject( "snowflake", l_x+10, l_y)
		setVelocityY( object, -0.33)
		setVelocityX( object, math.random()*0.24-0.12 )
	end
end

function collision( id, ...)
	
end
