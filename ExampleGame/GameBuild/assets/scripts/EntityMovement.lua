-- Chimera Script

local trans
local speed = 10.0
local jumpForce = 10
local movement = Vector2.new(0, 0)

function Init()

print("MOVEMENT")
trans = entity:GetTransformComponent()

local bob = GetEntityByName("Bob")
print (bob.name)
local script = bob:GetLuaScriptComponent(0)
print(script.fish)

--movement = Vector2.new(0, 0)

--print(entity.name)

end

function Update(ts)

	trans = entity:GetTransformComponent()
	local rb = entity:GetRigidBody2DComponent()
	movement.x = 0

	if (Input.IsKeyPressed(Key.A)) then
		--trans.position.x = trans.position.x - speed * ts

		movement.x = -1
	end

	if (Input.IsKeyPressed(Key.D)) then
		--trans.position.x = trans.position.x + speed * ts
		movement.x = 1
	end

	if(Input.IsKeyDown(Key.Space)) then
		rb.velocity = rb.velocity + Vector2.new(0, 1) * jumpForce
	end

	if (Input.IsKeyPressed(Key.K)) then
		local bob = GetEntityByName("Bob")
		local script = bob:GetLuaScriptComponent(0)
		print(script.KillMe("aurelian e prost"))
	end

	--if (Input.IsKeyPressed(Key.S)) then
		--trans.position.y = trans.position.y - speed * ts
	--end

	if (Input.IsKeyDown(Key.P)) then
		--print("Change scene")
		--sceneManager:LoadScene(1)
		CreateEmptyEntity("strunga")
	end
end

function FixedUpdate(fixedts)
	local rb = entity:GetRigidBody2DComponent()
	local moveVel = rb.velocity
	moveVel.x = movement.x * speed
	rb.velocity = moveVel
end

function CollisionEnter2D(contact)
	--print("pula")
	--print(contact.entity.name)
	
end