#pragma once
#include<Engine.h>
#ifdef ENGINE_PLATFORM_WINDOWS
ENGINE::Application* ENGINE::CreatApp();
int main(int argc, char** argv)
{
	ENGINE::Log::Init();
	LOG_CORE_TRACE("Core");
	LOG_TRACE("Client");
	auto app = ENGINE::CreatApp();
	app->Run();
	delete app;
}
#endif // ENGINE_PLATFORM_WINDOWS