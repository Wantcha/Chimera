-- Chimera Script

local totalTime = 60
local timeLeft
local progressBar
hasWon = false
hasLost = false

function Init()

	timeLeft = totalTime
	progressBar = GetEntityByName("TimeLeftBar")

end


function Update(ts)

	if hasWon == false and hasLost == false then
		progressBar:GetTransformComponent().scale = Vector3.new(12 * timeLeft / totalTime, 1, 1)
		timeLeft = timeLeft - ts

		if timeLeft <= 0 then

			hasWon = true
			SceneManager.LoadScene(2)

		end
	
	else
		if Input.IsKeyPressed(Key.Enter) then
			SceneManager.LoadScene(1)
		end
	end
	
end
