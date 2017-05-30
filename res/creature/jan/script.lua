package.path = package.path .. ";creature/human/?.lua"
require( "script")

local id_special_object = -1
local id_offset = 25

function special( id)
	if isAlive( id_special_object) == false then
		l_dir = getAnimationDirection( id)
		l_x, l_y = getPosition( id)
		l_velX, l_velY = getVelocity( id)
		l_factor = 0

		-- dir offset calc
		if l_dir == true then
			l_factor = -(16 + id_offset)
		end

		-- calc offset
		sid_x = id_offset + l_factor
		sid_y = 4
		
		--script.id_attack 
		id_special_object = createObject( "cry", l_x + sid_x, l_y + sid_y)
		setAnimationDirection( id_special_object, l_dir)
		setVelocityX( id_special_object, math.min(math.max( l_velX, -0.04), 0.04))
		setVelocityY( id_special_object, math.min(math.max( l_velY, -0.04), 0.04))
	end
end

function attack( id)
	if isAlive(getAttackId()) == false then
		l_dir = getAnimationDirection( id)
		l_x, l_y = getPosition( id)
		l_factor = 0

		-- dir offset calc
		if l_dir == true then
			l_factor = -25
		end
		-- calc offset
		id_x = 10 + l_factor
		id_y = 0
		
		--script.id_attack 
		id_attack = createObject( "shield", l_x + id_x, l_y + id_y)
		setAnimationDirection( id_attack, l_dir) -- dir
		setAttackPosition( id_x, id_y) -- offset
		setAttackId( id_attack) -- attach to the player
	end
end

