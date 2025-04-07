#pragma once
#include"Engine/Core.h"
#include"Engine/Scene/Scene.h"
//注册函数到ScriptEngine
namespace ENGINE
{
	class  ScriptEngine
	{
	public:
		ScriptEngine(Scene* scene);
		void Register();
		
	};

}