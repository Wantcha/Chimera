-- Chimera Script

local trans
local fish = "Bazinga"

function Init()
	Log.Warn("Starting")
	--trans = entity:GetTransformComponent()
	--trans.position.x = 0.23

	--parent = trans:GetParent()
	--print(parent.name)
	--camera = entity:GetCameraComponent()
	--print (camera:GetOrthoSize())
	--print(entity.name);
	--print( entity.ID )
end

function Update(ts)
	--trans = entity:GetTransformComponent()

	--if (Input.IsKeyPressed(Key.A)) then
		--trans.rotation.z = trans.rotation.z - 2.0 * ts
		--entity.name = "Pula"
	--end

	--if (Input.IsKeyPressed(Key.D)) then
		--trans.rotation.z = trans.rotation.z + 2.0 * ts
	--end
end

function KillMe(message)
	print(message)
end

function CollisionEnter2D(contact)
	--print("pula")
	print(contact.entity.name)
	
end