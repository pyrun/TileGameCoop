local flower_id = -1
local t_time = 0

function timer( id, time)
	if isAlive( flower_id) then
		local x, y = getPosition( id)
		local flowerx, flowery = getPosition( flower_id )
		local add = 0
		if y <= flowery+20 then
			if t_time > 40 then
				add = -0.3
				t_time = 0
			end
			addVelocity( flower_id, 0, -0.05 + add)
		end
		t_time = t_time + 1
	else
		local x,y = getPosition( id)
		flower_id = createObject( "blueflake", x, y)
	end
end