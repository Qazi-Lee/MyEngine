#pragma once
#include<Engine.h>
#ifdef ENGINE_PLATFORM_WINDOWS
ENGINE::Application* ENGINE::CreatApp();
int main(int argc, char** argv)
{
	ENGINE::Log::Init();
	auto app = ENGINE::CreatApp();
	app->Run();
	delete app;
}
#endif // ENGINE_PLATFORM_WINDOWS