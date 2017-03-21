function vertexhit( id)
end

local timer_time = 0

function start( id) 

end

function timer( id)
	timer_time = timer_time + 1
	if timer_time == 1 then
		delete( id)
	end
end

function collision( id, ...)
      for k,v in pairs({...}) do
	if isEnemy( v) == true then	
		kill( v)
	end
      end
end
