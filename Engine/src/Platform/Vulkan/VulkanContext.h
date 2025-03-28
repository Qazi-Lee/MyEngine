#pragma once
#include"Engine/Render/GraphicsContext.h"
#include"Platform/Vulkan/VulkanInstance.h"
#include"GLFW/glfw3.h"
namespace ENGINE
{
	class VulkanContext :public GraphicsContext
	{
	public:
		VulkanContext(GLFWwindow* windowHandle);
		void Init()override;
		void SwapBuffer()override;
		void* GetInstance()override;
		~VulkanContext();
	private:
		GLFWwindow* m_WindowHandle;
		VulkanInstance* m_VulkanInstance=nullptr;
	};


}