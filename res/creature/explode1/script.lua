local dead_timer = 13

function start( id)
	kill(id)
	play_sound( id, "explode")
end

function timer( id, time)
	dead_timer = dead_timer - 1
	if dead_timer == 0 then
		delete( id);
	end
end

function collision( id, ...)
      for k,v in pairs({...}) do
	if v == id then
	else
		kill( v);
	end
      end
end
