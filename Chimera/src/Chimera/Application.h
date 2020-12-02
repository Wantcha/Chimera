#pragma once

#include "Core.h"
#include "Window.h"
#include "Chimera/ImGui/ImGuiLayer.h"
#include "Chimera/LayerStack.h"
#include "Events/Event.h"

#include "Chimera/Events/ApplicationEvent.h"

#include "Chimera/Renderer/Shader.h"
#include "Chimera/Renderer/Buffer.h"
#include "Chimera/Renderer/VertexArray.h"
#include "Chimera/Renderer/OrthographicCamera.h"

#include "Chimera/Core/Timestep.h"

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

		inline static Application& Get(){ return *s_Instance; }

		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer; 
		bool m_Running = true;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}


