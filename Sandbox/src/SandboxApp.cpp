#include <Chimera.h>

class Sandbox : public Chimera::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Chimera::Application* Chimera::CreateApplication()
{
	return new Sandbox();
}