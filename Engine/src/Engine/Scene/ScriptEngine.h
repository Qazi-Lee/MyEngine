#pragma once
#include"Engine/Core.h"
#include"Engine/Scene/Scene.h"

#include<mono/jit/jit.h>
//注册函数到ScriptEngine
namespace ENGINE
{
	class  ScriptEngine
	{
	public:
		ScriptEngine(Scene* scene);
		void Register();
		void RegisterComponents(MonoImage* image);
		
	};

}