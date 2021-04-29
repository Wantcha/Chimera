#pragma once

#include "Core.h"
#include "Window.h"
#include "Chimera/ImGui/ImGuiLayer.h"
#include "Chimera/Core/LayerStack.h"
#include "Chimera/Events/Event.h"

#include "Chimera/Events/ApplicationEvent.h"

#include "Chimera/Renderer/Shader.h"
#include "Chimera/Renderer/Buffer.h"
#include "Chimera/Renderer/VertexArray.h"
#include "Chimera/Renderer/OrthographicCamera.h"
#include "Chimera/Renderer/Framebuffer.h"

#include "Chimera/Core/Timestep.h"

namespace Chimera {
	class CHIMERA_API Application
	{
	public:
		Application(const std::string& name = "Chimera App");
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get(){ return *s_Instance; }

		inline Window& GetWindow() { return *m_Window; }

		inline void SetGameWindowOffset(const glm::vec2& offset) { m_GameWindowOffset = offset; }
		glm::vec2 GetGameWindowOffset() { return m_GameWindowOffset; }

		void Close();

		//void InitializeGame();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
	private:
		bool OnWindowClosed(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnKeyReleased(KeyReleasedEvent& e);

		std::unique_ptr<Window> m_Window;

		glm::vec2 m_GameWindowOffset{0, 0};

		ImGuiLayer* m_ImGuiLayer; 
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
		const float m_FixedTimeStep = 1.0f/60.0f;
	private:
		static Application* s_Instance;
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}


