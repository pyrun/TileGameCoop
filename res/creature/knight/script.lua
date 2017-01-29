local wert = 0
local inflychange = 0
local change = 0
local maxchange = 40

function jump( id)
	if getColision( id, "down") then
		setVelocityY( id, -0.23)
		wert = 1
		inflychange = 1
		change = 0
	elseif wert == 1 then
		setVelocityY( id, -0.12)
		wert = 0
	end
end

local walk = 0.105
local flychange = 0.004

function right( id)
	if getColision( id, "down") then
		addVelocity( id, walk, 0 )
	elseif change < maxchange then
		change = change + 1
		local x, y = getVelocity( id)
		if x < 0 then
			setVelocityX( id, x/1.1)
		else
			addVelocity( id, flychange , 0)
		end
	end
end

function left( id)
	if getColision( id, "down") then
		addVelocity( id, -walk, 0 )
	elseif change < maxchange then
		change = change + 1
		local x, y = getVelocity( id)
		if x > 0 then
			setVelocityX( id, x/1.1)
		else
			addVelocity( id, -flychange , 0)
		end
	end
end

io.write(string.format("Hello from %s\n", _VERSION))