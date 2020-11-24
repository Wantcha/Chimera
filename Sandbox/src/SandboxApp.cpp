#include <Chimera.h>

#include "imgui/imgui.h"

class ExampleLayer : public Chimera::Layer
{
public:
	ExampleLayer()
		:Layer("Example")
	{}

	void OnUpdate() override
	{
		//CM_INFO("ExampleLayer::Update");

		if (Chimera::Input::IsKeyPressed(KEY_TAB))
			CM_TRACE("Tab key is pressed!");
	}

	void OnEvent(Chimera::Event& event) override
	{
		if (event.GetEventType() == Chimera::EventType::KeyPressed)
		{
			Chimera::KeyPressedEvent& e = (Chimera::KeyPressedEvent&)event;
			CM_TRACE("{0}", (char)e.GetKeyCode());
		}
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello boys");
		ImGui::End();
	}
};

class Sandbox : public Chimera::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Chimera::Application* Chimera::CreateApplication()
{
	return new Sandbox();
}