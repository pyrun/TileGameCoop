package.path = package.path .. ";creature/human/?.lua"
require( "script")

local cooldown_time = 20

function attack( id)
	if isAlive(getAttackId()) == false and getCooldown() == 0 then
		l_dir = getAnimationDirection( id)
		l_x, l_y = getPosition( id)
		l_factor = 0
		if l_dir == true then
			l_factor = -64
		end
		id_x = 16 + l_factor
		id_y = 8
		
		setCooldown( cooldown_time)
		
		--script.id_attack =
		id_attack = createObject( "electronics whip", l_x + id_x, l_y + id_y);
		setAnimationDirection( id_attack, l_dir)
		setAttackPosition( id_x, id_y)
		setAttackId( id_attack)
	end
end

