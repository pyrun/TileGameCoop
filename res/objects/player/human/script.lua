local inLiquid = false
local max_default_speed = 0.06
local max_speed = max_default_speed
local run_speed = max_default_speed * 1.7

local walk_speed = 0.03
local jump_two = 0
local jump_high = -0.32
local jump_outwater_factor = 0.75

function vertexhit( id)
	if getColision( id, "down") then
		jump_two = 1
	end
end

-- Attack
local id_attack = -1
local id_attack_x = 0
local id_attack_y = 0
function getAttackId()
	return id_attack
end
function setAttackId( id)
	id_attack = id
end
function setAttackPosition( posx, posy) 
	id_attack_x = posx
	id_attack_y = posy
end
-- Special
local id_special = -1
local id_special_x = 0
local id_special_y = 0
function getSpecialId()
	return id_special
end
function setSpecialId( id)
	id_special = id
end
function setSpecialPosition( posx, posy) 
	id_special_x = posx
	id_special_y = posy
end

-- wasser
function liquid( id, swim)
	if isAlive( id) == false then
		do return end
	end
	inLiquid = swim
	if swim == false then
		-- get velocity
		l_velX, l_velY = getVelocity( id)
		
		-- lets look if you fast enough to jump out water
		if l_velY < -0.5 then
			-- jump of the water
			setVelocityY( id, jump_high*jump_outwater_factor)
		else
			-- dont jump out
			setVelocityY( id, 0)
		end
	end
end

function update( id)
	local l_velX, l_velY

	if isAlive( id) == false then
		do return end
	end
	
	if isAlive( id_attack) then
		l_x, l_y = getPosition( id)
		setPosition( id_attack, l_x + id_attack_x, l_y + id_attack_y)
	end

	if isAlive( id_special) then
		l_x, l_y = getPosition( id)
		setPosition( id_special, l_x + id_special_x, l_y + id_special_y)
	end

  	-- get velocity
	l_velX, l_velY = getVelocity( id)

	if math.abs(l_velX) > 0.0 then
		local dir = false
		if l_velX < 0.00 then
			dir = true
		end
		setAnimationDirection( id, dir)
	end
	
	if inLiquid == true then
		setAnimation( id, "swim")
		do return end
	end

	if l_velY < 0.0 then
		setAnimation( id, "jump")
	end
	
	if getColision( id, "down") and
		( getAnimation( id) == "idle" or
		  getAnimation( id) == "jump" or
		  getAnimation( id) == "walk" or
		  getAnimation( id) == "run" )then
		if math.abs(l_velX) > 0.01 then
			setAnimation( id, "walk")
		end
		if math.abs(l_velX) < 0.01 then
			setAnimation( id, "idle")
		end
	end
end

function start( id )
	setVelocityX( id, -0.0001)
end



function jump( id)
	if isAlive( id) == false then
		do return end
	end
	if inLiquid then
		setVelocityY( id, jump_high*2)
		do return end
	end
	if getColision( id, "down") then
		addVelocity( id, 0, jump_high)
		play_sound( id, "jump")
		jump_two = 1
    elseif jump_two == 1 then
		jump_two = 0
		setVelocityY( id, jump_high/2)
	end
end

function up( id) 
	l_x, l_y = getPosition( id)

	offset_x = 10
	offset_y = 0

	l_rect_x = 14
	l_rect_y = 32


	ids = {findObjects( id, l_x + offset_x, l_y + offset_y, l_rect_x, l_rect_y)}

	for object_id = 1, #ids do
		local obj = ids[object_id]
		sendSignal( obj, id, "enter")
	end
end

function down( id) 

end

function right( id)
	if isAlive( id) == false then
		do return end
	end
	local l_velX, l_velY
    	-- get velocity
	l_velX, l_velY = getVelocity( id)
	if l_velX < max_speed then
		addVelocity( id, walk_speed, 0 )
	end
end

function left( id)
	if isAlive( id) == false then
		do return end
	end
    	local l_velX, l_velY
	-- get velocity
	l_velX, l_velY = getVelocity( id)
	if l_velX > -max_speed then
    		addVelocity( id, -walk_speed, 0 )
	end
end

function run( id, press)
	if isAlive( id) == false then
		do return end
	end
	if press and getColision( id, "down") then
		max_speed = run_speed
	else
		max_speed = max_default_speed
	end
end

