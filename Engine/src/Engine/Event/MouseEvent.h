#pragma once

#include<Engine/Event/Event.h>

namespace ENGINE {
	class  MouseMovedEvent :public Event {
	public:
		MouseMovedEvent(double x,double y):
			m_MouseX(x),m_MouseY(y){}
		double GetX()const { return m_MouseX; }
		double GetY()const { return m_MouseY; }
		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent:" << m_MouseX << "," << m_MouseY;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseMove);
	private:
		double m_MouseX;
		double m_MouseY;
};
	class  MouseScrolledEvent :public Event
	{
	public:
		MouseScrolledEvent(double x,double y):
			deltaX(x),deltaY(y){}
		double GetX()const { return deltaX; }
		double GetY()const { return deltaY; }
		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent:" << deltaX << "," << deltaY;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseScrolled);
	private:
		double deltaX, deltaY;
	};
	class  MouseButtonEvent :public Event
	{
	public:
		int GetButton()const { return m_Button; }
	protected:
		MouseButtonEvent(int button):
			m_Button(button){}
		int m_Button;
	};
	class  MousePressedEvent:public MouseButtonEvent{
	public:
		MousePressedEvent(int button,bool isrepeat):
			MouseButtonEvent(button),m_IsRepeat(isrepeat){}
		bool IsRepeat()const { return m_IsRepeat; }
		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "MousePresseEvent:" << m_Button;
			if (IsRepeat())
			{
				ss << "(Repeat)";
			}
			return ss.str();
		}
		EVENT_CLASS_TYPE(MousePressed);
	private:
		bool m_IsRepeat = false;
	};
	class  MouseReleasedEvent :public MouseButtonEvent {
	public:
		MouseReleasedEvent(int button) :
			MouseButtonEvent(button) {}
		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "MouseReleaseEvnet:" << m_Button;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseRelease);
	};
}
