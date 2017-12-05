function start( id)
	global_value = -1
end

local fade = 1

function timer( id)
	fade = fade+33

	if fade > 255 then
		fade = 255
	end
end

function signal( id, fromId, data)
	if data == "reset" then
		fade = 128
	end

	if data == "enter" then
		if fade < 255 then
			do return end
		end

		l_x, l_y = getPosition( tonumber(global_value))

		setPosition( fromId, l_x, l_y)

		sendSignal( tonumber(global_value), id, "reset")
		-- cooldown
		fade = 128
	end
end