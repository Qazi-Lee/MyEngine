#pragma once
#include<pch.h>

#include"Core.h"
#include<Engine/Event/Event.h>
//创建一个抽象的窗口类,支持多平台
namespace ENGINE {
	struct WindowPros
	{
		std::string Title;
		int Width;
		int Height;
		WindowPros(const std::string& title = "MyEngine", int width = 1280, int height = 720)
			:Title(title), Width(width), Height(height){}
	};
	class  Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;//声明函数类型
		virtual ~Window() {};
		virtual void OnUpdate() = 0;
		//const 修饰成员函数，成员函数不能修改属性
		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
		virtual void* GetNativeWindow()const = 0;
		//参数为默认值
		static Window* Creat(const WindowPros& windowpros=WindowPros());
		virtual void SetEventCallbackFn(const EventCallbackFn& callback)=0;
	};

}