package.path = package.path .. ";creature/human/?.lua"
require( "script")

local id_special_object = -1
local id_offset = 14

function start( id )
end

function attack( id)
	if isAlive( id_special_object) == false then
		l_dir = getAnimationDirection( id)
		l_x, l_y = getPosition( id)

		l_factor = 0

		-- dir offset calc
		if l_dir == true then
			l_factor = -(-8 + id_offset)
		end

		-- calc offset
		id_x = id_offset + l_factor
		id_y = 24
		
		--script.id_attack 
		id_special_object = createObject( "bomb", l_x + id_x, l_y + id_y)
		setVelocityY( id_special_object, -0.05)
	elseif isAlive( id_special_object) == true and getAnimation( id_special_object) == "idle" then
		setAnimation( id_special_object, "explode")
	end
end

function special( id)
	if getAnimation( id) == "idle" then
		setAnimation( id, "punch" )
	end
	if getAnimation( id) == "walk" then
		setAnimation( id, "walk_punch" )
	end
end

function punch( id )
	l_x, l_y = getPosition( id)

	offset_x = 10
	offset_y = 10

	l_rect_x = 14
	l_rect_y = 24


	ids = {findObjects( id, l_x + offset_x, l_y + offset_y, l_rect_x, l_rect_y)}

	for object_id = 1, #ids do
		local obj = ids[object_id]
		sendSignal( obj, id, "press")
	end

	setAnimation( id, "idle" )
end