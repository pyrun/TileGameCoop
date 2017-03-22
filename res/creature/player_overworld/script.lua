local move_speed = 0.1
local move_maxsteps = 16
local isMoving = false
local move_ysteps=0
local move_xsteps=0
local move_inverted=false
local move_y_flag=false

function vertexhit( id)
end

function attack( id)
end

function liquid( id, swim)

end

function update( id)

end


function jump( id)

end

function setPositionTo16( id)
	l_x, l_y = getPosition( id)
	l_x = math.floor( l_x/16.0)*16
	l_y = math.floor( l_y/16.0)*16
	setPosition( id, l_x, l_y)
end

function timer( id, time)
	l_x, l_y = getPosition( id)
	if isMoving == true then
		if move_inverted == false then
			if move_y_flag == false and move_xsteps < l_x then
				isMoving = false
				setVelocityX( id, 0)
				l_x, l_y = getPosition( id)
				setPosition( id, move_xsteps, l_y)
			end
			if move_y_flag == true and move_ysteps < l_y then
				isMoving = false
				l_x, l_y = getPosition( id)
				setPosition( id, l_x, move_ysteps)
				setVelocityY( id, 0)
			end
		else
			if move_y_flag == false and move_xsteps > l_x then
				isMoving = false
				l_x, l_y = getPosition( id)
				setPosition( id, move_xsteps, l_y)
				setVelocityX( id, 0)
			end
			if move_y_flag == true and move_ysteps > l_y then
				isMoving = false
				l_x, l_y = getPosition( id)
				setPosition( id, l_x, move_ysteps)
				setVelocityY( id, 0)
			end
		end
	end
end

function up( id) 
	if isMoving == false then
		isMoving = true
		l_x, l_y = getPosition( id)
		l_x = math.floor( l_x/16.0)*16
		l_y = math.floor( l_y/16.0)*16
		move_ysteps = l_y - (move_maxsteps)
		move_inverted = true
		addVelocity( id, 0, -move_speed)
		move_y_flag = true
	end
end

function down( id)
	if isMoving == false then
		isMoving = true
		l_x, l_y = getPosition( id)
		l_x = math.floor( l_x/16.0)*16
		l_y = math.floor( l_y/16.0)*16
		move_ysteps = l_y + (move_maxsteps)
		move_inverted = false
		addVelocity( id, 0, move_speed)
		move_y_flag = true
	end
end

function right( id)
	if isMoving == false then
		isMoving = true
		l_x, l_y = getPosition( id)
		l_x = math.floor( l_x/16.0)*16
		l_y = math.floor( l_y/16.0)*16
		move_xsteps = l_x + (move_maxsteps)
		move_inverted = false
		addVelocity( id, move_speed, 0)
		move_y_flag = false
	end
end

function left( id)
	if isMoving == false then
		isMoving = true
		l_x, l_y = getPosition( id)
		l_x = math.floor( l_x/16.0)*16
		l_y = math.floor( l_y/16.0)*16
		move_xsteps = l_x - (move_maxsteps)
		move_inverted = true
		addVelocity( id, -move_speed, 0)
		move_y_flag = false
	end
end

function run( id, press)

end

