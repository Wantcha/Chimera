#pragma once

#include "Chimera/Core/Core.h"
#include "Chimera/Core/Keycodes.h"
#include "Chimera/Core/MouseButtonCodes.h"

namespace Chimera
{
	class CHIMERA_API Input
	{
	public:
		static bool IsKeyPressed(int keycode);

		static bool IsMouseButtonPressed(int button);
		static float GetMouseX();
		static float GetMouseY();
		static std::pair<float, float> GetMousePosition();
	};
}
