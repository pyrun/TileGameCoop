function timer( id)
	if getFrame( id) == 5 then
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
