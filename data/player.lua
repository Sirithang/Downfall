collided = false

function onUpdate()
	
	local x,y = DF.getPosition(this)
	local fx,fy = DF.getDirection(this)
	local angle = DF.getAngle(this);

	local nx, ny= x,y;

	if DF.isKeyPressed(KEYS.Z) then
		nx = x + fx * 0.1
		ny = y + fy * 0.1
	elseif DF.isKeyPressed(KEYS.S) then
		nx = x - fx * 0.1
		ny = y - fy * 0.1
	end

	if not DF.sphereCollide(nx,ny,0.5) then
		DF.setPosition(this, nx, ny)
	end

	if DF.isKeyPressed(KEYS.Q) then
		DF.setAngle(this, angle + 0.05)
	elseif DF.isKeyPressed(KEYS.D) then
		DF.setAngle(this, angle - 0.05)
	end
	
	collided = false

end

function onCreate()
	print("created")
end

function onCollision(id)
	print("collision "..id);
	collided = true
end