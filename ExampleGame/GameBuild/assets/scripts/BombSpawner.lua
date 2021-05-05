-- Chimera Script

local time = 0
local timeToSpawnNextBomb = 2.3
local cameraEntity

function Init()

cameraEntity = GetEntityByName("Camera")

end

function Update(ts)

	if entity:GetLuaScriptComponent(1).hasLost == false then
		time = time + ts
		if time > timeToSpawnNextBomb then
			--print("oy")
			time = 0
			if timeToSpawnNextBomb > 0.65 then
				timeToSpawnNextBomb = timeToSpawnNextBomb - 0.045
			end
			SpawnBomb()

		end
	end

end

function SpawnBomb()

	local bomb = SpawnWrap("\\Bomb.wrap")
	local camera = cameraEntity:GetCameraComponent()
	local trans = bomb:GetTransformComponent()

	local posX = camera:ScreenToWorld( Vector2.new( camera:GetWidth(), 0 ), cameraEntity:GetTransformComponent():GetTransform() ).x
	trans.position = Vector3.new( posX + 2, trans.position.y, -0.5)

end