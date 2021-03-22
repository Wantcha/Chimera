-- Chimera Script

local trans
local speed = 3.0

function Init()

print("MOVEMENT")
trans = entity:GetTransformComponent()

end

function Update(ts)

	if (Input.IsKeyPressed(Key.A)) then
		trans.position.x = trans.position.x - speed * ts
	end

	if (Input.IsKeyPressed(Key.D)) then
		trans.position.x = trans.position.x + speed * ts
	end

	if (Input.IsKeyPressed(Key.W)) then
		trans.position.y = trans.position.y + speed * ts
	end

	if (Input.IsKeyPressed(Key.S)) then
		trans.position.y = trans.position.y - speed * ts
	end
end

function CollisionEnter2D(contact)
	--print("pula")
	print(contact.entity.name)
	
end