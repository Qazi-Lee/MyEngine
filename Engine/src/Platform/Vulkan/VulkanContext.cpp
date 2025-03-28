#include"pch.h"

#include"VulkanContext.h"


namespace ENGINE
{
	VulkanContext::VulkanContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle)
	{
		m_VulkanInstance = new VulkanInstance(m_WindowHandle);
	}
	void VulkanContext::Init()
	{
		m_VulkanInstance->Init();
	}
	void VulkanContext::SwapBuffer()
	{
		m_VulkanInstance->mainloop();
	}
	void* VulkanContext::GetInstance()
	{
		return m_VulkanInstance;
	}
	VulkanContext::~VulkanContext()
	{
		if (m_VulkanInstance != nullptr)
		{
			m_VulkanInstance->Cleanup();
			delete m_VulkanInstance;
		}
		m_VulkanInstance = nullptr;
	}
}