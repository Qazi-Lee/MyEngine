#include "pch.h"
#include "OpenglContext.h"

#include<glad/glad.h>
#include<GLFW/glfw3.h>

namespace ENGINE
{
	OpenglContext::OpenglContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle)
	{
	}

	void OpenglContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if (success)
		{
			LOG_CORE_INFO("glad初始化成功");
		}
	}

	void OpenglContext::SwapBuffer()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

	void* OpenglContext::GetInstance()
	{
		return nullptr;
	}

}
