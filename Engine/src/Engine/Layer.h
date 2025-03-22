#pragma once
#include<Engine/Core.h>
#include<Engine/Event/Event.h>
namespace ENGINE
{
	class  Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();
		virtual void OnAttach(){}
		virtual void OnDetach(){}
		virtual void OnUpdate(Time t){}
		virtual void OnEvent(Event&e){}
		virtual void OnImGuiRender() {}
		//返回值不可修改且不修改成员
		inline const std::string GetName() const { return Layer_name; }
	private:
		std::string Layer_name;
    };
}
