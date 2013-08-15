collided = false
size = 0.4

function onUpdate()
	
	local x,y = DF.getPosition(this)
	local fx,fy = DF.getDirection(this)
	local angle = DF.getAngle(this);

	local nx, ny= x,y;
	
	local speed = 0.05

	if DF.isKeyPressed(KEYS.W) then
		nx = x + fx * speed
		ny = y + fy * speed
	elseif DF.isKeyPressed(KEYS.S) then
		nx = x - fx * speed
		ny = y - fy * speed
	end

	if DF.sphereCollide(nx, y, size) then nx = x end
	if DF.sphereCollide(x, ny, size) then ny = y end
	
	DF.setPosition(this, nx, ny)

	if DF.isKeyPressed(KEYS.A) then
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