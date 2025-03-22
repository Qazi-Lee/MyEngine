#include"pch.h"
#include"Engine/Time.h"
#include"Render/Render.h"
#include"GLFW/glfw3.h"
namespace ENGINE
{
	 float Time::GetTime()
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::API::None: return 0.0f;
		case RenderAPI::API::OpenGL: return glfwGetTime();
		}
		return 0.0f;
	}
}