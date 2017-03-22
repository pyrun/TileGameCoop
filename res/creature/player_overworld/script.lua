local move_speed = 0.1
local move_maxsteps = 16
local isMoving = false
local move_ysteps=0
local move_xsteps=0
local move_inverted=false

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

function up( id) 

end

function down( id) 

end

function right( id)
	if isMoving == false then
		isMoving = true
		move_xsteps = move_maxsteps
		move_inverted = false
	end
end

function left( id)
	if isMoving == false then
		isMoving = true
		move_xsteps = move_maxsteps
		move_inverted = true
	end
end

function run( id, press)

end

