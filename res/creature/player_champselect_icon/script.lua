function timer( id, time)

end

function collision( id, ...)
	for k,v in pairs({...}) do
		if getPlayer( v) > 0 then
			if getPlayerChamp( getPlayer( v)) == "" then	
				setPlayerChamp( "alex", getPlayer( v))
			end
		end
	end
end
