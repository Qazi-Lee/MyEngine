#pragma once
#include"Engine/Render/GraphicsContext.h"
#include"GLFW/glfw3.h"
namespace ENGINE
{
	class VulkanContext :public GraphicsContext
	{
	public:
		VulkanContext(GLFWwindow* windowHandle);
		void Init()override;
		void SwapBuffer()override;
	private:
		GLFWwindow* m_WindowHandle;
	};


}