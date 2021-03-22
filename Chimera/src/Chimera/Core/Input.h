#pragma once

#include "Chimera/Core/Core.h"
#include "Chimera/Core/Keycodes.h"
#include "Chimera/Core/MouseButtonCodes.h"

namespace Chimera
{
	class Input
	{
	public:
		static bool IsKeyDown(int keycode);
		static bool IsKeyReleased(int keycode);
		static bool IsKeyPressed(int keycode);
		
		static void SetKeyPressed(int keycode, bool value)
		{
			m_PressedKeys[keycode] = value;
		}

		static bool IsMouseButtonPressed(int button);
		static float GetMouseX();
		static float GetMouseY();
		static std::pair<float, float> GetMousePosition();
	private:
		static bool m_PressedKeys[255];
	};
}
