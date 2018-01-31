local object1 = -1
local object2 = -1
local object3 = -1

int_xpos = {}
int_ypos = {}

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

	-- get all right !
	setDepth( id, 1000+id)
end

function timer( id, time)
	if isAlive( id) == false then
		do return end
	end

	pos_x, pos_y = getPosition( id)

	table.insert( int_xpos, pos_x)
	table.insert( int_ypos, pos_y)

	velx, vely = getVelocity( id)

	-- create body objects
	if isAlive( object1) == false then
		object1 = createObject( "cloud_walker_body", pos_x, pos_y)
	end
	if isAlive( object2) == false then
		object2 = createObject( "cloud_walker_body", pos_x, pos_y)
	end
	if isAlive( object3) == false then
		object3 = createObject( "cloud_walker_body", pos_x, pos_y)
	end

	local offset = 3
	local factor = 4

	if #int_xpos > factor*3+offset then
		table.remove( int_xpos, 1)
		table.remove( int_ypos, 1)
	end

	if #int_xpos < factor then
		setPosition( object1, int_xpos[#int_xpos], int_ypos[#int_xpos])
		setPosition( object2, int_xpos[#int_xpos], int_ypos[#int_xpos])
		setPosition( object3, int_xpos[#int_xpos], int_ypos[#int_xpos])
	elseif #int_xpos < factor*2 then
		setPosition( object1, int_xpos[factor], int_ypos[factor])
		setPosition( object2, int_xpos[#int_xpos], int_ypos[#int_xpos])
		setPosition( object3, int_xpos[#int_xpos], int_ypos[#int_xpos])
	elseif #int_xpos < factor*3 then
		setPosition( object1, int_xpos[factor], int_ypos[factor])
		setPosition( object2, int_xpos[factor*2], int_ypos[factor*2])
		setPosition( object3, int_xpos[#int_xpos], int_ypos[#int_xpos])
	else
		setPosition( object1, int_xpos[factor], int_ypos[factor])
		setPosition( object2, int_xpos[factor*2], int_ypos[factor*2])
		setPosition( object3, int_xpos[factor*3], int_ypos[factor*3])
	end

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

	if math.random( 100) == 1 then
		addVelocity( id, 0, -0.2)
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
