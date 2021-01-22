#pragma once

//#ifdef CM_PLATFORM_WINDOWS

extern Chimera::Application* Chimera::CreateApplication();

int main(int argc, char** argv)
{
	Chimera::Log::Init();

	CM_PROFILE_BEGIN_SESSION("Startup", "ChimeraProfile-Startup.json");
	auto app = Chimera::CreateApplication();
	CM_PROFILE_END_SESSION();

	CM_PROFILE_BEGIN_SESSION("Runtime", "ChimeraProfile-Runtime.json");
	app->Run();
	CM_PROFILE_END_SESSION();

	CM_PROFILE_BEGIN_SESSION("Shutdown", "ChimeraProfile-Shutdown.json");
	delete app;
	CM_PROFILE_END_SESSION();
}

//#endif

