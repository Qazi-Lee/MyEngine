#include "pch.h"
#include "WindowsWindow.h"

#include<Engine/Event/KeyEvent.h>
#include<Engine/Event/MouseEvent.h>
#include<Engine/Event/WindowEvent.h>

#include"Platform/OpenGL/OpenglContext.h"
#include"Platform/Vulkan/VulkanContext.h"


namespace ENGINE {

	Window* Window::Creat(const WindowPros& pros)
	{
		return new WindowsWindow(pros);
	}
	WindowsWindow::WindowsWindow(const WindowPros& pros)
	{
		Init(pros);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}
	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffer();
	}

	void WindowsWindow::Init(const WindowPros& pros)
	{

		m_Data.Title = pros.Title;
		m_Data.Height = pros.Height;
		m_Data.Width = pros.Width;
		LOG_CORE_TRACE("Creat Window {0},({1},{2})", pros.Title, pros.Width, pros.Height);
		int success = glfwInit();
		if (success)
		{
			LOG_CORE_INFO("glfw初始化成功");
		}
		switch (m_API)
		{
		case GraphicsAPI::OPENGL:
			OpenglInit(); break;
		case GraphicsAPI::VULKAN:
			VulkanInit(); break;
		case GraphicsAPI::DIRECTX12:
			DirectXInit(); break;
		default:
			OpenglInit(); break;
		}
		//创建上下文
		m_Context->Init();
		glfwSetWindowUserPointer(m_Window, &m_Data);
		//设置回调函数
		/*按键函数*/
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, scancode, 0);
				data.m_CallbackFn(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleaseEvent event(key, scancode);
				data.m_CallbackFn(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, scancode, true);
				data.m_CallbackFn(event);
				break;
			}
			}
			});
		/*窗口函数*/
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.m_CallbackFn(event);
			});
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;
			WindowResizeEvent event(width, height);
			data.m_CallbackFn(event);
			});
		/*鼠标函数*/
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
			case GLFW_PRESS: {
				MousePressedEvent event(button, 0);
				data.m_CallbackFn(event);
				break;
			}
			case GLFW_RELEASE: {
				MouseReleasedEvent event(button);
				data.m_CallbackFn(event);
				break;
			}
			case GLFW_REPEAT: {
				MousePressedEvent event(button, true);
				data.m_CallbackFn(event);
				break;
			}
			}
			});
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event(xoffset, yoffset);
			data.m_CallbackFn(event);
			});
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent event(xpos, ypos);
			data.m_CallbackFn(event);
			});
		/*文本函数*/
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t codepoint)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				KeyTypedEvent event(codepoint, 0);
				data.m_CallbackFn(event);
			});
		return;
	}
	void WindowsWindow::Shutdown()
	{

		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}
	void WindowsWindow::OpenglInit()
	{
#ifdef GLFW_INCLUDE_VULKAN
#undef GLFW_INCLUDE_VULKAN
#endif 
		m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), NULL, NULL);
		m_Context = new OpenglContext(m_Window);
	}
	void WindowsWindow::VulkanInit()
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), NULL, NULL);
		m_Context = new VulkanContext(m_Window);
	}
	void WindowsWindow::DirectXInit()
	{
	}

}
