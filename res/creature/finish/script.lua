global_value_2 = "1"

savedone = false

function timer( id, time)
	if savedone == false and global_value_3 == nil then
		--savestate()
		savedone = true
	end
	-- check if enough player still life
	if getAmountPlayerChamps() < tonumber(global_value_2) then
		setLoadLevel( "gameover.tmx", true)
	end
end


function collision( id, ...)
	player = 0

	for k,v in pairs({...}) do
		if isAlive( v) == true and isPlayer( v) == true then	
			player = player + 1
		end
	end

	if player == tonumber(global_value_2) then
		setLoadLevel( global_value, true )
	end
end
