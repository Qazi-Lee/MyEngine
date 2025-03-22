#pragma once

#include<Engine/Event/Event.h>

namespace ENGINE {
	class  KeyEvent :public Event
	{
	public:
		int GetKeyCode()const { return m_KeyCode; }
		int GetScanCode()const { return m_ScanCode; }
	protected:
		KeyEvent(const int keycode,const int scancode) :
			m_KeyCode(keycode),m_ScanCode(scancode){}
		int m_KeyCode;
		int m_ScanCode;
	};
	class  KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const int keycode, const int scancode,bool isrepeat) :
			KeyEvent(keycode,scancode), m_IsRepeat(isrepeat) {}
		bool IsRepeat()const { return m_IsRepeat; }
		std::string ToString()const override {
			std::stringstream ss;
			ss << "KeyPressedEvent:" << m_KeyCode;
			if (IsRepeat())
			{
				ss << "(Repeat)";
			}
			return ss.str();
		}
		//可将以下函数的实现用宏代替
		// EVENT_CLASS_TYPE
		//static EventType GetStaticType() { return EventType::KeyPressed;}
		//EventType GetType()const override { return GetStaticType(); }
		//const char* GetName()const override { return "KeyPressed"; }
		EVENT_CLASS_TYPE(KeyPressed);

	private:
		bool m_IsRepeat=false;
	};
	class  KeyReleaseEvent:public KeyEvent {
	public:
		KeyReleaseEvent(const int keycode,const int scancode):
			KeyEvent(keycode,scancode){}
		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent:" << m_KeyCode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyRelease);
	};
	class  KeyTypedEvent :public KeyEvent
	{
	public:
		KeyTypedEvent(const int keycode,const int scancode) :
			KeyEvent(keycode,scancode) {}
		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent:" << m_KeyCode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyTyped);
	};
}