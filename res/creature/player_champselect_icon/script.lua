function timer( id, time)
	local amount = math.floor(tonumber(getPlayerEntityAmount( getAnimation( id))))
	print( id, tostring(amount) )
end

function collision( id, ...)
	for k,v in pairs({...}) do
		if getPlayer( v) > 0 then
			if getPlayerChamp( getPlayer( v)) == "" and getPlayerEntityAmount( getAnimation( id)) > 0 then	
				setPlayerChamp( getAnimation( id), getPlayer( v))
				removePlayerEntity( getAnimation( id))
			end
		end
	end
end
