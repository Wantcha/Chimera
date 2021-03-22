#include "cmpch.h"
#include "Application.h"

#include "Input.h"

#include "Chimera/Renderer/Renderer.h"
#include "Chimera/Core/Keycodes.h"
#include "Chimera/Assets/AssetManager.h"
#include "Chimera/Scripting/LuaManager.h"
#include "Chimera/Scripting/LuaScriptComponent.h"

#include <GLFW/glfw3.h>

namespace Chimera {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		CM_PROFILE_FUNCTION();

		CM_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = Window::Create(WindowProps(name));
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		AssetManager::RegisterCache(CreateScope<AssetCache<Texture2D>>());
		//AssetManager::RegisterCache(CreateScope<AssetCache<LuaScriptComponent>>());

		LuaManager::Get().InitBindings();
	}

	Application::~Application()
	{
		CM_PROFILE_FUNCTION();
	}

	void Application::PushLayer(Layer* layer)
	{
		CM_PROFILE_FUNCTION();
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		CM_PROFILE_FUNCTION();
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnEvent(Event& e)
	{
		CM_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(OnKeyPressed));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(OnKeyReleased));

		//CM_CORE_TRACE("{0}", e);

		//Why not go from back to front? TODO: Look into this
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	void Application::Run()
	{
		CM_PROFILE_FUNCTION();
		float timeIncrement = 0.0f;
		float fixedUpdateTime = 0.0f;
		while (m_Running)
		{
			CM_PROFILE_SCOPE("RunLoop");
			float time = (float)glfwGetTime(); //Platform::GetTime
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			timeIncrement += timestep;
			if (timeIncrement > 2.5f)
			{
				AssetManager::CollectGarbage();
				timeIncrement = 0.0f;
			}

			if (!m_Minimized)
			{
				CM_PROFILE_SCOPE("LayerStack OnUpdate");
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}
			fixedUpdateTime += timestep;
			if(fixedUpdateTime > m_FixedTimeStep)
			{
				CM_PROFILE_SCOPE("LayerStack OnFixedUpdate");
				for (Layer* layer : m_LayerStack)
					layer->OnFixedUpdate(m_FixedTimeStep);
				fixedUpdateTime -= m_FixedTimeStep;
			}


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

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		CM_PROFILE_FUNCTION();
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

	bool Application::OnKeyPressed(KeyPressedEvent& e)
	{
		Input::SetKeyPressed(e.GetKeyCode(), e.GetRepeatCount() < 1);
		//CM_CORE_ERROR(e.GetRepeatCount());

		return false;
	}

	bool Application::OnKeyReleased(KeyReleasedEvent& e)
	{
		Input::SetKeyPressed(e.GetKeyCode(), false);
		//CM_CORE_ERROR(e.GetRepeatCount());

		return false;
	}

}
