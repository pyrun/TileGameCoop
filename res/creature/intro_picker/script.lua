item_id = -1

function next_item( id)
	ids = {findObjects( id, 0, 0, 1000, 1000)}
	for object_id = 1, #ids do
		local obj = ids[object_id]
		if getName( obj) == "intro_button" and not (tonumber(item_id) == tonumber(obj) ) then
			item_id = obj
			l_x, l_y = getPosition( item_id)
			setPosition( id, l_x, l_y)
			do return end
		end
	end
end

function start( id)
	ids = {findObjects( id, 0, 0, 800, 800)}
	for object_id = 1, #ids do
		local obj = ids[object_id]
		if getName( obj) == "intro_button" then
			--item_id = obj
			--l_x, l_y = getPosition( item_id)
			--setPosition( id, l_x, l_y)
		end
	end
end

function attack( id)
	kill(item_id)
end

function liquid( id, swim)

end

function update( id)

end

function jump( id)

end


function up_once( id) 
	next_item( id)
end

function down_once( id)
	next_item( id)
end

function right_once( id)
	next_item( id)
end

function left_once( id)
	next_item( id)
end

function run( id, press)

end

