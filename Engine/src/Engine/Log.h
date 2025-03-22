#pragma once
#include"Core.h"
#include"spdlog/spdlog.h"
#include"spdlog/sinks/stdout_color_sinks.h"  
namespace ENGINE {                      
	class  Log
	{
	public:
		Log() {}
		~Log() {}
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> CoreLogger;
		static std::shared_ptr<spdlog::logger> ClientLogger;
	};
}
/**************************引擎日志宏定义*********************************/
#define LOG_CORE_TRACE(...)  ::ENGINE::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LOG_CORE_INFO(...)   ::ENGINE::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_CORE_WARN(...)   ::ENGINE::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_CORE_ERROR(...)  ::ENGINE::Log::GetCoreLogger()->error(__VA_ARGS__)
/**************************客户日志宏定义*********************************/
#define LOG_TRACE(...)  ::ENGINE::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)   ::ENGINE::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)   ::ENGINE::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)  ::ENGINE::Log::GetClientLogger()->error(__VA_ARGS__)
