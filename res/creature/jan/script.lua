package.path = package.path .. ";creature/human/?.lua"
require( "script")

function special( id)
-- place_portal
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
	else
		if getAnimation( getAttackId() ) then
			setAnimation( getAttackId(), "idle")
		end
	end
end

