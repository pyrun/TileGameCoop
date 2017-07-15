package.path = package.path .. ";creature/human/?.lua"
require( "script")

local portal_red = -1
local portal_green = -1
local portal_set = 0

function special( id)
	if isAlive( portal_green) == true and portal_set == 1 then
		delete( portal_green)
		do return end
	end
	if isAlive( portal_red) == true and portal_set == 0 then
		delete( portal_red)
		do return end
	end
	
	if isAlive( portal_red) == false and portal_set == 0 then
		l_x, l_y = getPosition( id)
		l_dir = getAnimationDirection( id)
		
		-- dir offset calc
		if l_dir == true then
			l_factor = 10
		else 
			l_factor = 10
		end

		portal_red = createObject( "place_portal", l_x + l_factor, l_y)
		setAnimationDirection( portal_red, l_dir) -- dir
		setAnimation( portal_red, "red")
		portal_set = 1
		do return end
	end
	if isAlive( portal_green) == false and portal_set == 1 then
		l_x, l_y = getPosition( id)
		l_dir = getAnimationDirection( id)
		
		-- dir offset calc
		if l_dir == true then
			l_factor = 10
		else 
			l_factor = 10
		end

		portal_green = createObject( "place_portal", l_x + l_factor, l_y)
		setAnimationDirection( portal_green, l_dir) -- dir
		setAnimation( portal_green, "green")
		portal_set = 0
		do return end
	end
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

