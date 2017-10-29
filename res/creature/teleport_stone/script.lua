local owner = -1
local timer_time = 10

function start( id)
	setAlpha( id, 0)
end
local fade = 1

function timer( id, time)
	timer_time = timer_time - 1

	fade = fade+10

	if fade > 255 then
		fade = 255
	end

	setAlpha( id, fade)
end

function signal( id, fromId, data)
	if data == "transfer" then
		owner = fromId
	end
end

function vertexhit( id)
	if getColision( id, "down") then
		if timer_time < 0 then
			velX, velY = getPosition( id)
			setPosition( owner, velX+1, velY)
		end
		delete( id)
	end
end
