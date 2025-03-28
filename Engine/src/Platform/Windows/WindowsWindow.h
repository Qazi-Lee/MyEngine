#pragma once
#include<Engine/Window.h>
#include"Engine/Render/GraphicsContext.h"
#include<GLFW/glfw3.h>
#define GLFW_INCLUDE_VULKAN
namespace ENGINE {


	class  WindowsWindow :public Window
	{
	public:
		WindowsWindow(const WindowPros& pros);
		virtual ~WindowsWindow();
		void OnUpdate()override;
		inline int GetWidth() const override { return m_Data.Width; }
		inline int GetHeight() const override { return m_Data.Height; }
		//为窗口绑定一个回调函数，该回调函数应该由App设置
		void SetEventCallbackFn(const EventCallbackFn& callback)override { m_Data.m_CallbackFn = callback; }
		inline void* GetNativeWindow()const override { return m_Window; };
		inline void* GetContext()const override { return m_Context; }
		inline GraphicsAPI GetGraphicsAPI()const override { return m_API; }
	private:
		 void Init(const WindowPros& pros);
		 void Shutdown();

		 void OpenglInit();
		 void VulkanInit();
		 void DirectXInit();

	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;
		struct WindowData
		{
			std::string Title;
			int Width;
			int Height;
			EventCallbackFn m_CallbackFn;
		}m_Data;
	public:
		GraphicsAPI m_API= GraphicsAPI::OPENGL;
		
	};
}