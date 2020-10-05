#pragma once

#include "Core.h"

namespace Chimera {
	class CHIMERA_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}


