collided = false
size = 0.4

function onUpdate()
	
	local x,y = DF.getPosition(this)
	local fx,fy = DF.getDirection(this)
	local angle = DF.getAngle(this);

	local nx, ny= x,y;
	
	local speed = 5

	if DF.isKeyPressed(KEYS.W) then
		nx = x + fx * speed * gDeltaTime
		ny = y + fy * speed * gDeltaTime
	elseif DF.isKeyPressed(KEYS.S) then
		nx = x - fx * speed * gDeltaTime
		ny = y - fy * speed * gDeltaTime
	end

	if DF.sphereCollide(nx, y, size) then nx = x end
	if DF.sphereCollide(x, ny, size) then ny = y end
	
	DF.setPosition(this, nx, ny)

	if DF.isKeyPressed(KEYS.A) then
		DF.setAngle(this, angle + 5.0 * gDeltaTime)
	elseif DF.isKeyPressed(KEYS.D) then
		DF.setAngle(this, angle - 5.0 * gDeltaTime)
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