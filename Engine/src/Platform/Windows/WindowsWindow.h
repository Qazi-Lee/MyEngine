#pragma once
#include<Engine/Window.h>
#include"Platform/OpenGL/OpenglContext.h"
#include<GLFW/glfw3.h>
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
	private:
		virtual void Init(const WindowPros& pros);
		virtual void Shutdown();
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
	};
}