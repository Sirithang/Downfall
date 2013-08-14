function onUpdate()
	
	local x,y = DF.getPosition(this)
	local fx,fy = DF.getDirection(this)
	
	if DF.isKeyPressed(KEYS.W) then
		DF.setPosition(this, x, y)
	elseif DF.isKeyPressed(KEYS.S) then
		DF.setPosition(this, x, y)
	end
	
end

function onCreate()
	print("created")
end