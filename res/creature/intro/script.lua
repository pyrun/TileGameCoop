function start( id)

end

function timer( id)
	local find_intro_text = false
	ids = {findObjects( id, 0, 0, 1000, 1000)}
		for object_id = 1, #ids do
			local obj = ids[object_id]
			if getName( obj) == "textbox" then
				find_intro_text = true
			end
		end
	if find_intro_text == false then
		setLoadLevel( "labor.tmx", true )
	end
end
