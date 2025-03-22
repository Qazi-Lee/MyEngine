#pragma once
#include<memory>
#include"Engine/Time.h"
//宏定义
#ifdef ENGINE_PLATFORM_WINDOWS
	//#ifdef ENGINE_BUILD_DLL
	//	#define ENGINE_API _declspec(dllexport)
	//#else
	//	#define ENGINE_API _declspec(dllimport)
	//#endif // ENGINE_BUILD_DLL
#else
	#error Only Windows!
#endif // ENGINE_PLATFORM_WINDOWS
#define BIND_EVENT_FN(fn) std::bind(&fn,this,std::placeholders::_1)
namespace ENGINE
{
	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T>
	using Scope = std::unique_ptr<T>;

}
