package.path = package.path .. ";creature/human/?.lua"
require( "script")

local thow_speed = 0.4

function special( id )
	if getAnimation( id) == "walk" or getAnimation( id) == "idle" then
		setAnimation( id, "throw" )

		l_x, l_y = getPosition( id)
		l_vel_x, l_vel_y = getVelocity( id)
		offset_x = 8
		offset_y = 12
		l_rect_x = 16
		l_rect_y = 16

		ids = {findObjects( id, l_x + offset_x, l_y + offset_y, l_rect_x, l_rect_y)}

		for object_id = 1, #ids do
			local obj = ids[object_id]
			if getName( obj) == "alex" or
				getName( obj) == "jan" or
				getName( obj) == "spring" or
				getName( obj) == "bomb"
				then
				setVelocityX( obj, l_vel_x)
				setVelocityY( obj, -thow_speed)
			end
		end
	end
end

function throw( id )
	setAnimation( id, "idle" )
end


function attack( id)
	
	if isAlive(getAttackId()) == false then
		l_dir = getAnimationDirection( id)
		l_x, l_y = getPosition( id)
		l_factor = 0

		-- dir offset calc
		if l_dir == true then
			l_factor = -32
		end
		-- calc offset
		id_x = 16 + l_factor
		id_y = 8
		
		--script.id_attack 
		id_attack = createObject( "whip", l_x + id_x, l_y + id_y)
		setAnimationDirection( id_attack, l_dir) -- dir
		setAttackPosition( id_x, id_y) -- offset
		setAttackId( id_attack) -- attach to the player
	end
end
