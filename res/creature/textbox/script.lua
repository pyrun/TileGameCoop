local fade = 0

function start( id)
	setAlpha( id, 0)
end

function timer( id) 
	if fade < 255 and isAlive(id) then
		fade = fade + 10
		if fade > 255 then
			fade = 255
			kill(id)
		end
		setAlpha( id, fade)
	elseif fade > 0 and not isAlive(id) then
		fade = fade - 10
		if fade < 0 then
			fade = 0
			delete( id)
			do return end
		end
		setAlpha( id, fade)
	end
end
