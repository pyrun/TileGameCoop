local move_speed = 0.05
local move_maxsteps = 16
local isMoving = false
local move_ysteps=0
local move_xsteps=0
local move_inverted=false
local move_y_flag=false
local move_fix = 2 
local move_back = false

function vertexhit( id)
end

function attack( id)
	setAnimation( id, "join")
end

function liquid( id, swim)

end

function update( id)

end


function jump( id)
	setAnimation( id, "join")
end

function setPositionTo16( id)
	l_x, l_y = getPosition( id)
	l_x = math.floor( l_x/16.0)*16
	l_y = math.floor( l_y/16.0)*16
	setPosition( id, l_x, l_y)
end

function timer( id, time)
	l_x, l_y = getPosition( id)
	if getAnimation( id ) == "join" then
		setAnimation( id, "idle")
	end
	if isMoving == true then
		if move_inverted == false then
			if move_y_flag == false and move_xsteps-move_fix < l_x then
				isMoving = false
				setVelocityX( id, 0)
				l_x, l_y = getPosition( id)
				setPosition( id, move_xsteps, l_y)
			end
			if move_y_flag == true and move_ysteps-move_fix < l_y then
				isMoving = false
				l_x, l_y = getPosition( id)
				setPosition( id, l_x, move_ysteps)
				setVelocityY( id, 0)
			end
		else
			if move_y_flag == false and move_xsteps+move_fix > l_x then
				isMoving = false
				l_x, l_y = getPosition( id)
				setPosition( id, move_xsteps, l_y)
				setVelocityX( id, 0)
			end
			if move_y_flag == true and move_ysteps+move_fix > l_y then
				isMoving = false
				l_x, l_y = getPosition( id)
				setPosition( id, l_x, move_ysteps)
				setVelocityY( id, 0)
			end
		end
	end
end

function up( id) 
	l_x1, l_y1 = getPosition( id)
	tile_x = l_x1/16
	tile_y = l_y1/16
	solid_left, solid_right, solid_down, solid_up, isliq, tileid = getTile( tile_x, tile_y)

	local velx, vely = getVelocity( id)
	if math.abs(velx) < 0.01 and math.abs(vely) < 0.01 and isMoving == true then
		if move_inverted == false and move_y_flag == true then
			addVelocity( id, 0, move_speed)
			move_inverted = true
		end
	end

	if isMoving == false and solid_up == false then
		l_x, l_y = getPosition( id)
		isMoving = true
		l_x = math.floor( l_x/16.0)*16
		l_y = math.floor( l_y/16.0)*16
		move_ysteps = l_y - (move_maxsteps)
		move_inverted = true
		addVelocity( id, 0, -move_speed)
		move_y_flag = true
	end
end

function down( id)
	l_x1, l_y1 = getPosition( id)
	tile_x = l_x1/16
	tile_y = l_y1/16
	solid_left, solid_right, solid_down, solid_up, isliq, tileid = getTile( tile_x, tile_y)

	local velx, vely = getVelocity( id)
	if math.abs(velx) < 0.01 and math.abs(vely) < 0.01 and isMoving == true then
		if move_inverted == true and move_y_flag == true then
			addVelocity( id, 0, -move_speed)
			move_inverted = false
		end
	end

	if isMoving == false and solid_down == false then
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
	l_x1, l_y1 = getPosition( id)
	tile_x = l_x1/16
	tile_y = l_y1/16
	solid_left, solid_right, solid_down, solid_up, isliq, tileid = getTile( tile_x, tile_y)

	local velx, vely = getVelocity( id)
	if math.abs(velx) < 0.01 and math.abs(vely) < 0.01 and isMoving == true then
		if move_inverted == true and move_y_flag == false then
			addVelocity( id, -move_speed, 0)
			move_inverted = false
		end
	end

	if isMoving == false and solid_right == false then
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
	l_x1, l_y1 = getPosition( id)
	tile_x = l_x1/16
	tile_y = l_y1/16
	solid_left, solid_right, solid_down, solid_up, isliq, tileid = getTile( tile_x, tile_y)

	local velx, vely = getVelocity( id)
	if math.abs(velx) < 0.01 and math.abs(vely) < 0.01 and isMoving == true then
		if move_inverted == false and move_y_flag == false then
			addVelocity( id, move_speed, 0)
			move_inverted = true
		end
	end

	if isMoving == false and solid_left == false then
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

