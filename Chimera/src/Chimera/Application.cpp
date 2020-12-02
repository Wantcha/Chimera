#include "cmpch.h"
#include "Application.h"

#include "Input.h"

#include "Chimera/Renderer/Renderer.h"
#include "Chimera/Keycodes.h"

#include <GLFW/glfw3.h>

namespace Chimera {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		CM_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		
	}

	Application::~Application()
	{
		
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClosed));
		//dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(OnKeyPressed));

		//CM_CORE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float time = (float)glfwGetTime(); //Platform::GetTime
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();

			m_ImGuiLayer->End();

			m_Window->OnUpdate();
			
		}
	}


	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	/*bool Application::OnKeyPressed(KeyPressedEvent& e)
	{
		glm::vec3 camPos = m_Camera.GetPosition();

		if (e.GetKeyCode() == KEY_W)
		{
			camPos.y += 0.1f;
			m_Camera.SetPosition(camPos);
		}

		if (e.GetKeyCode() == KEY_S)
		{
			camPos.y -= 0.1f;
			m_Camera.SetPosition(camPos);
		}

		if (e.GetKeyCode() == KEY_A)
		{
			camPos.x -= 0.1f;
			m_Camera.SetPosition(camPos);
		}
		if (e.GetKeyCode() == KEY_D)
		{
			camPos.x += 0.1f;
			m_Camera.SetPosition(camPos);
		}

			
		return true;
	}*/

}
