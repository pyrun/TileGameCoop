package.path = package.path .. ";text/?.lua"
local text_lib = nil
local fade = 0

function start( id)
	setAlpha( id, 0)
	setPosition( id, 40, 170)
end

local fade_finish = false
local next_text = false

function mysplit(inputstr, sep)
        if sep == nil then
                sep = "%s"
        end
        local t={} ; i=1
        for str in string.gmatch(inputstr, "([^"..sep.."]+)") do
                t[i] = str
                i = i + 1
        end
        return t
end

local text_lifetime = 10
local text_move = 0
local text_sentence = 0
local text_offset = { 14, 10}

function timer( id)
	-- get lib
	if text_lib == nil then
		text_lib = require(global_value)
		text_lib.table_start()
	end
	if fade < 255 and isAlive(id) then
		fade = fade + 10
		if fade > 255 then
			fade = 255
			fade_finish = true
		end
		setAlpha( id, fade)
	elseif fade > 0 and not isAlive(id) then
		fade = fade - 10
		if fade < 0 then
			fade = 0
			delete( id)
			do return end
		end
		setAlpha( id, fade)
	end

	if isAlive( id) == false then
		do return end
	end

	if fade_finish == true then
		-- next sentence
		if next_text == true then
			text_sentence = text_sentence + 1
			text_move = 0
			next_text = false
		end

		text = text_lib.get( text_sentence+1 )
		
		if text == "END" then
			kill(id)
			do return end
		end

		if text == "ENTER" then
			for variable = 1, text_sentence+1, 1 do
				text_lib.del( 1)
			end
			text_move = 0
			text_sentence = 0
			do return end
		end
		
		local text_length = string.len( text )
		
		text_move = text_move + 1
		if text_length < text_move then
			text_move = text_move - 1
		end
		
		for variable = 1, text_sentence, 1 do
			message( id, 1.5, text_offset[1], text_offset[2] + ( 16 * (variable - 1)), true, text_lifetime, text_lib.get( variable ) )
		end
		
		text_cut = string.sub( text, 0, text_move)
		message( id, 1.5, text_offset[1], text_offset[2] + ( 16 * text_sentence) , true, text_lifetime, text_cut )
	end
end

function attack( id)
	next_text = true
end

