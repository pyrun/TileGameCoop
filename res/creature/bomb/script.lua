function explode( id)
	l_x, l_y = getPosition( id)
	createObject( "explode0", l_x-5, l_y-5)
	delete( id)
end