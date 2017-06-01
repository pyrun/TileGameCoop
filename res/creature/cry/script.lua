local dead_timer = 13

function start( id)
end

function action_die( id)
	delete( id)
end

function collision( id, ...)
      for k,v in pairs({...}) do
	if v == id then
	elseif isEnemy( v) == true then
		setAnimationDirection( v, not getAnimationDirection( v))
	end
      end
end
