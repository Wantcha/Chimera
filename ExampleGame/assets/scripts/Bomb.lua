-- Chimera Script

local minSpeed = 3.0
local maxSpeed = 4.0
local speed
local rotationSpeed = 0.5

local cameraEntity
local gameManager

function Init()

	gameManager = GetEntityByName("GameManager")
	cameraEntity = GetEntityByName("Camera")
	local character = GetEntityByName("Character")
	local characterY = character:GetTransformComponent().position.y

	--print (cameraEntity.name)
	local camera = cameraEntity:GetCameraComponent()
	math.randomseed(os.time()) -- random initialize
	math.random(); math.random(); math.random()
	local trans = entity:GetTransformComponent()

	local posYMax = camera:ScreenToWorld( Vector2.new( 0, camera:GetHeight() ), cameraEntity:GetTransformComponent():GetTransform() ).y - 0.3
	local posYMin = camera:ScreenToWorld( Vector2.new( 0, 0 ), cameraEntity:GetTransformComponent():GetTransform() ).y + 0.4

	trans.position = Vector3.new( trans.position.x, RandomFloat(posYMin, posYMax), trans.position.z)

	-- We try to lerp a little towards the character's Y position
	local curY = trans.position.y
	local desiredY = curY + (characterY - curY) * RandomFloat(0.4, 0.75)
	trans.position = Vector3.new( trans.position.x, desiredY, trans.position.z)

	local randomScale = RandomFloat(1.1, 1.5)
	trans.scale = Vector3.new( trans.scale.x * randomScale, trans.scale.y * randomScale, trans.scale.z )
	entity:GetCircleCollider2DComponent().radius = entity:GetCircleCollider2DComponent().radius * randomScale

	speed = RandomFloat(minSpeed, maxSpeed)
	trans.rotation = Vector3.new(trans.rotation.x, trans.rotation.y, RandomFloat(0, 6))
	rotationSpeed = RandomFloat(0.5, 1)

end

function RandomFloat(lower, greater)
	return lower + math.random() * (greater - lower)
end

function Update(ts)

	if gameManager:GetLuaScriptComponent(1).hasLost == false then
		local trans = entity:GetTransformComponent()
		local camera = cameraEntity:GetCameraComponent()

		trans.position = Vector3.new(trans.position.x - speed * ts, trans.position.y, trans.position.z)
		trans.rotation = Vector3.new(trans.rotation.x, trans.rotation.y, trans.rotation.z  + rotationSpeed * ts)

		local posX = camera:ScreenToWorld( Vector2.new( 0, 0 ), cameraEntity:GetTransformComponent():GetTransform() ).x
		if(trans.position.x < posX - 1) then
		
			--print(trans.position.x)
			Destroy(entity)

		end
	end

end


--function CollisionEnter2D(contact)

	
--end