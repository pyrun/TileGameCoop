local set_once = false

function timer( id, time)
	if set_once == false then
		addPlayerEntity( "alex")
		set_once = true
	end
end

function collision( id, ...)
	
end
