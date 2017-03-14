local flower_id = 0
local time_flow = 0

function timer( id, time)
	shootX = { }
	shootY = { }
	--shootX[0] = 0
	shootY[0] =-0.5
	time_flow = time_flow + 1

	if time_flow == 10 then
		time_flow = 0
	end

	for i=0, 10 do
		shootX[i] = i/2;
	end

	for i=0, 10 do
		shootY[i] = (i-5)/10;
	end

	if isAlive( id) == true then
		local x, y = getPosition( id)
		flower_id = createObject( "blueflake", x, y)
		setVelocityY( flower_id, shootY[time_flow])
		setVelocityX( flower_id, 0)
		-- shootX[ tonumber(time_flow) ], 
	end
end
