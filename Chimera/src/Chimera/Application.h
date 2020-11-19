#pragma once

#include "Core.h"
#include "Window.h"
#include "Chimera/LayerStack.h"
#include "Events/Event.h"

#include "Chimera/Events/ApplicationEvent.h"

namespace Chimera {
	class CHIMERA_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
	private:
		bool OnWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}


