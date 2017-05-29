package.path = package.path .. ";creature/human/?.lua"
require( "script")

local id_special_object = -1
local id_offset = 30

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
		id_x = id_offset + l_factor
		id_y = 8
		
		--script.id_attack 
		id_special_object = createObject( "solidbox", l_x + id_x, l_y + id_y)
		setVelocityX( id_special_object, l_velX)
		setVelocityY( id_special_object, l_velY)
	elseif isAlive( id_special_object) == true and getAnimation( id_special_object) == "free" then
		delete( id_special_object)
	end
end

function attack( id)
	
	if isAlive(getAttackId()) == false then
		l_dir = getAnimationDirection( id)
		l_x, l_y = getPosition( id)
		l_factor = 0

		-- dir offset calc
		if l_dir == true then
			l_factor = -64
		end
		-- calc offset
		id_x = 16 + l_factor
		id_y = 8
		
		--script.id_attack 
		id_attack = createObject( "electronics whip", l_x + id_x, l_y + id_y)
		setAnimationDirection( id_attack, l_dir) -- dir
		setAttackPosition( id_x, id_y) -- offset
		setAttackId( id_attack) -- attach to the player
	end
end

