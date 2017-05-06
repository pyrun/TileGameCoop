local inLiquid = false
local max_speed = 0.1
local walk_speed = 0.03
local jump_two = 0
local jump_high = -0.32
local jump_outwater_factor = 0.75

function vertexhit( id)

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
	return id_attack
end
function setSpecialId( id)
	id_attack = id
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
		setPosition( id_attack, l_x + id_x, l_y + id_y)
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
	
	if getColision( id, "down") then
		if math.abs(l_velX) > 0.01 then
			setAnimation( id, "walk")
		end
		if math.abs(l_velX) < 0.01 then
			setAnimation( id, "idle")
		end
	end
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
		jump_two = 1
    	elseif jump_two == 1 then
		jump_two = 0
		setVelocityY( id, jump_high/2)
	end
end

function up( id) 
	io.write("up\n")
end

function down( id) 
	io.write("down\n")
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
		max_speed = 0.1*1.5
	else
		max_speed = 0.1
	end
end

