#pragma once
#include<pch.h>
#include<Engine/Core.h>
namespace ENGINE {
/*****注册事件类型****/
enum  class  EventType
{
	None = 0,
	//窗口变化
	WindowResize,WindowClose, 
	//鼠标输入
	MousePressed,MouseRelease,MouseMove, MouseScrolled,
	//键盘输入
	KeyPressed,KeyRelease,KeyTyped
};

//用宏来代替Event虚函数的实现
#define EVENT_CLASS_TYPE(type)    		static EventType GetStaticType() { return EventType::type;}\
                                        EventType GetType()const override { return GetStaticType(); }\
                                        const char* GetName()const override { return #type; }
class   Event
{
public:
	virtual EventType GetType() const =0;
	virtual const char* GetName() const = 0;
	//将事件信息打印
	virtual std::string ToString() const { return GetName(); }
	bool m_Handle=true;
};
//事件调度类
class  EventDispatcher 
{
public:
	EventDispatcher(Event&event):
		m_Event(event){}
	//模板T为传入的事件类型，模板F为传入的调用的函数,F由传入的函数来推断
	template<typename T,typename F>
	bool Dispatch(const F&func)
	{
		if (m_Event.GetType() == T::GetStaticType())
		{
			m_Event.m_Handle|=func(static_cast<T&>(m_Event));
			return true;
		}
		return false;
	}
private:
	Event& m_Event;
};
}

