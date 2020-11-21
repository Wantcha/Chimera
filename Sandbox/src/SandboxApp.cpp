#include <Chimera.h>

class ExampleLayer : public Chimera::Layer
{
public:
	ExampleLayer()
		:Layer("Example")
	{}

	void OnUpdate() override
	{
		CM_INFO("ExampleLayer::Update");
	}

	void OnEvent(Chimera::Event& event) override
	{
		CM_TRACE("{0}", event);
	}
};

class Sandbox : public Chimera::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Chimera::ImGuiLayer());
	}

	~Sandbox()
	{

	}
};

Chimera::Application* Chimera::CreateApplication()
{
	return new Sandbox();
}