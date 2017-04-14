local flower_id = 0
local time_flow = 0

function timer( id, time)
	shootX = { }
	shootY = { }
	time_flow = time_flow + 3

	if time_flow > 10 then
		time_flow = time_flow - 10
	end

	for i=-5, 5 do
		shootX[i] = (i*0.01);
	end

	for i=0, 10 do
		shootY[i] = -0.4;
	end

	if isAlive( id) == true then
		local x, y = getPosition( id)
		flower_id = createObject( "blueflake", x, y-8)
		addVelocity( flower_id, shootX[time_flow-5], shootY[time_flow])
	end
end
