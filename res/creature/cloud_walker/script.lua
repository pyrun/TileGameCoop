local object1 = -1
local object2 = -1
local object3 = -1

local int_x1, int_y1
local int_x2, int_y2
local int_x3, int_y3
local int_x4, int_y4

local counter = 0

local speed = 0.032

function vertexhit( id)
	if getVertexHit( id, 1) then
		setAnimationDirection( id, false)
	end
	if getVertexHit( id, 2) then
		setAnimationDirection( id, true)
	end

	down1 = getVertexHit( id, 3)
	down2 = getVertexHit( id, 4)

	if down1 == false and down2 == true then
		setAnimationDirection( id, false)
	end
	if down1 == true and down2 == false then
		setAnimationDirection( id, true)
	end
end


function start( id) 
	setAnimation( id, "walk")
end

function timer( id, time)
	if isAlive( id) == false then
		do return end
	end

	pos_x, pos_y = getPosition( id)
	velx, vely = getVelocity( id)

	counter = 1 + counter
	if counter > 2 then
		counter = 0
	end

	if counter == 2 then
		int_x4 = int_x3
		int_y4 = int_y3
		int_x3 = int_x2
		int_y3 = int_y2
		int_x2 = int_x1
		int_y2 = int_y1
		int_x1, int_y1 = getPosition( id)
	end

	-- create body objects
	if isAlive( object1) == false then
		int_x2, int_y2 = getPosition( id)
		object1 = createObject( "cloud_walker_body", pos_x, pos_y)
	end
	if isAlive( object2) == false then
		int_x3, int_y3 = getPosition( id)
		object2 = createObject( "cloud_walker_body", pos_x, pos_y)
	end
	if isAlive( object3) == false then
		int_x4, int_y4 = getPosition( id)
		object3 = createObject( "cloud_walker_body", pos_x, pos_y)
	end

	setPosition( object1, int_x2, int_y2)
	setPosition( object2, int_x3, int_y3)
	setPosition( object3, int_x4, int_y4)

	-- walk
	if getAnimation( id) == "walk" or getAnimation( id) == "idle" then
		if getGravity( id) == false then
			setGravity( id, true)
		end

		if getColision( id, "down") then
			local dir = getAnimationDirection( id)
			setAnimation( id, "walk")
			if dir == false then
				addVelocity( id, speed, 0)
			else
				addVelocity( id, -speed, 0)
			end
		end
	end

	-- richtung anzeigen
	if math.abs(velx) > 0.0 then
		local dir = false
		if velx < 0.00 then
			dir = true
		end
		setAnimationDirection( id, dir)
	end
end

function collision( id, ...)
	if isAlive( id) == false then
		do return end
	end
	for k,v in pairs({...}) do
		if isAlive( v) == true and isPlayer( v) == true then
			kill( v)
		end
	end
end
