#pragma once

#ifdef CM_PLATFORM_WINDOWS

extern Chimera::Application* Chimera::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Chimera::CreateApplication();
	app->Run();
	delete app;
}

#endif

