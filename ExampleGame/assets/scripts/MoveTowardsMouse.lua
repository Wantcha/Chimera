-- Chimera Script

local speed = 2.8
local cameraEntity
local dashSpeed = 12.0
local dashDuration = 0.13
local dashTime = 0.0
local dashCoolDown = 1.0
local isDashing = false
local gameManager = GetEntityByName("GameManager")

function Init()

	cameraEntity = GetEntityByName("Camera")
	gameOverPanel = GetEntityByName("Lose")
	gameManager = GetEntityByName("GameManager")

end

function Update(ts)

	if gameManager:GetLuaScriptComponent(1).hasLost == false then

		local trans = entity:GetTransformComponent()

		local camera = cameraEntity:GetCameraComponent()

		dashTime = dashTime + ts
		if Input.IsKeyPressed(Key.Space) and dashTime > dashCoolDown then
			dashTime = 0
			isDashing = true
		end

		if isDashing then

			trans.position = trans.position + trans:GetForward() * dashSpeed * ts
			if dashTime > dashDuration then

				dashTime = 0
				isDashing = false

			end

		else
			local mousePosX, mousePosY = Input.GetMousePosition()

			local camTrans = cameraEntity:GetTransformComponent():GetTransform()
			local worldPoint = camera:ScreenToWorld(Vector2.new(mousePosX, mousePosY), camTrans)

			lowerLeftBound = camera:ScreenToWorld( Vector2.new( 0, 0 ), cameraEntity:GetTransformComponent():GetTransform() )
			upperRightBound = camera:ScreenToWorld( Vector2.new( camera:GetWidth(), camera:GetHeight() ), cameraEntity:GetTransformComponent():GetTransform() )

			if math.abs(Vec3.Distance(trans.position, worldPoint)) > 0.35 then
				newRotation = trans.rotation + Vec3.RotateTowards(trans:GetForward(), worldPoint - trans.position)

				trans.rotation = newRotation
				trans.position = trans.position + trans:GetForward() * speed * ts
			end

		end

		pos = trans.position
		if pos.x < lowerLeftBound.x then
			trans.position = Vector3.new( lowerLeftBound.x, pos.y, pos.z )
		end

		if pos.x > upperRightBound.x then
			trans.position = Vector3.new( upperRightBound.x, pos.y, pos.z)
		end

		if pos.y < lowerLeftBound.y then
			trans.position = Vector3.new( pos.x, lowerLeftBound.y, pos.z)
		end

		if pos.y > upperRightBound.y then
			trans.position = Vector3.new( pos.x, upperRightBound.y, pos.z)
		end

	end
	
end

function SensorEnter2D(collider)

	local countDown = gameManager:GetLuaScriptComponent(1)

	if countDown.hasWon == false and countDown.hasLost == false then

		countDown.hasLost = true
		--print("LOST")
		gameOverPanel.enabled = true

	end
end