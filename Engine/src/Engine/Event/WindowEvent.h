#pragma once
#include"pch.h"
#include<Engine/Event/Event.h>
namespace ENGINE {
	class  WindowCloseEvent :public Event
	{
	public:
		WindowCloseEvent() = default;
		//继承父类函数
		//std::string ToString()const override
		//{
		//	std::stringstream ss;
		//	ss << "WindowCloseEvent";
		//	return ss.str();
		//}
		EVENT_CLASS_TYPE(WindowClose);
	};
	class  WindowResizeEvent :public Event
	{
	public:
		WindowResizeEvent(int width,int height):
			m_Width(width),m_Height(height){}
		int GetWidth()const { return m_Width; }
		int GetHeight()const { return m_Height; }
		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent:" << m_Width << "," << m_Height;
			return ss.str();
		}
		EVENT_CLASS_TYPE(WindowResize);
	private:
		int m_Width;
		int m_Height;
   };
}
