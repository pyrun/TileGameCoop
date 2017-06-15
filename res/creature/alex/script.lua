--package.path = package.path .. ";creature/human/?.lua"
--require( "script")

local move_speed = 0.02

function vertexhit( id)
end

function attack( id)

end

function liquid( id, swim)

end

function update( id)

end


function jump( id)

end

function up( id) 
	addVelocity( id, 0, -move_speed)
end

function down( id)
	addVelocity( id, 0, move_speed)
end

function right( id)
	addVelocity( id, move_speed, 0)
end

function left( id)
	addVelocity( id, -move_speed, 0)
end

function run( id, press)

end

