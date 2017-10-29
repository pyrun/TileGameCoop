package.path = package.path .. ";creature/human/?.lua"
require( "script")

local portal_red = -1
local portal_green = -1
local portal_set = 0

local arrow = 90

local angle_deg = 0
local angle_steps = 5

local offset_arrow_x = 12
local offset_arrow_y = 10

function up( id) 
	l_x, l_y = getPosition( id)

	ids = {findObjects( id, l_x, l_y, 32, 32)}

	for object_id = 1, #ids do
		local obj = ids[object_id]
		if getName( obj) == "place_portal" then
			sendSignal( obj, id, "transfer")
		end
	end
end

function timer( id )
	if isAlive(getSpecialId()) == true then
		if angle_deg < -30 then
			angle_steps = -angle_steps
		elseif angle_deg > 220 then
			angle_steps = -angle_steps
		end
		angle_deg = angle_deg + angle_steps
		local l_angle = (angle_deg)* math.pi / 180 + math.pi
		vel_x = math.cos( l_angle ) * 13
		vel_y = math.sin( l_angle ) * 13

		setSpecialPosition( vel_x + offset_arrow_x, vel_y +offset_arrow_y)

		if angle_deg > 180 then
			setAnimation( getSpecialId(), "rightdown")
		elseif angle_deg > 150 then
			setAnimation( getSpecialId(), "right")
		elseif angle_deg > 110 then
			setAnimation( getSpecialId(), "rightup")
		elseif angle_deg > 70 then
			setAnimation( getSpecialId(), "up")
		elseif angle_deg > 40 then
			setAnimation( getSpecialId(), "leftup")
		elseif angle_deg > 0 then
			setAnimation( getSpecialId(), "left")
		elseif angle_deg > -45 then
			setAnimation( getSpecialId(), "leftdown")
		else
			angle_deg = 0
		end
	end
end

function special( id)

	if isAlive(getSpecialId()) == false then
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
		id_attack = createObject( "arrow", l_x + id_x, l_y + id_y)
		setSpecialPosition( id_x, id_y) -- offset
		setSpecialId( id_attack) -- attach to the player
		angle_deg = 90
		update( id) -- once update graphic
	else
		delete( getSpecialId() )
		l_x, l_y = getPosition( id)

		local l_angle = (angle_deg + 180)* math.pi / 180
		speed_x = math.cos( l_angle) * 0.5
		speed_y = math.sin( l_angle) * 0.5
		local shoot_object = createObject( "teleport_stone", l_x, l_y )
		setVelocityX( shoot_object, speed_x)
		setVelocityY( shoot_object, speed_y)
		sendSignal( shoot_object, id, "transfer")
	end

	--[[
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
]]--
end


local speed_temp = walk_speed

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

