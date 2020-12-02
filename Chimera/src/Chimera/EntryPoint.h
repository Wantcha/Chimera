#pragma once

#ifdef CM_PLATFORM_WINDOWS

extern Chimera::Application* Chimera::CreateApplication();

int main(int argc, char** argv)
{
	Chimera::Log::Init();
	CM_CORE_WARN("Initialized Log");

	auto app = Chimera::CreateApplication();
	app->Run();
	delete app;
}

#endif

