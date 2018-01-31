deathtime = 4

function timer( id, time)
	if getAnimation(id) == "explode" or getAnimation(id) == "die" then
		l_x, l_y = getPosition( id)
		deathtime = deathtime - 1
		if deathtime > 0 then
			message( id, 1, 0, 0, false, time-10, tostring(deathtime) )
		end
		if deathtime == 0 then
			createObject( "explode0", l_x-5, l_y-5)
			delete( id)
		end
	end
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
