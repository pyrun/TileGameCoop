function explode( id)
	l_x, l_y = getPosition( id)
	createObject( "explode0", l_x-5, l_y-5)
	delete( id)
end

local becare = false

function signal( id, fromId, data)
	if data == "throw" then
		becare = true
	end
end

function vertexhit( id)
	if becare == true then
		kill( id)
	end
end
