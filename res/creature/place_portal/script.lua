function start( id)
	setAlpha( id, 0)
end

local fade = 1

function timer( id)
	fade = fade+33

	if fade > 255 then
		fade = 255
	end

	setAlpha( id, fade)
end

local portal = -1

function signal( id, fromId, data)
	if data == "transfer" then
		fade = 128
	end
end


function find_portal( id)
	local animation = "green"
	if getAnimation( id) == "green" then
		animation = "red"
	end
	ids = {findObjects( id, 0, 0, 100000000, 100000000)}

	for object_id = 1, #ids do
		local obj = ids[object_id]
		if getName( obj) == getName( id) and getAnimation( obj) == animation then
			portal = obj
		end
	end
end

function collision( id, ...)
	if fade < 255 then
		do return end
	end
	
	if isAlive( id) == false then
		do return end
	end
	
	if isAlive( portal) == false then
		find_portal( id)
	end
	if isAlive( portal) == false then
		do return end
	end
	
	l_x, l_y = getPosition( portal)
	
	for k,v in pairs({...}) do
		if isAlive( v) == true then
			if isPlayer( v) == true then
				sendSignal( portal, id, "transfer")
				setPosition( v, l_x, l_y)
			end
		end
	end
	fade = 128
end
