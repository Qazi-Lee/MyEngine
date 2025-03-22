#include"pch.h"
#include "Log.h"
namespace ENGINE {
	std::shared_ptr<spdlog::logger>Log::CoreLogger;
	std::shared_ptr<spdlog::logger>Log::ClientLogger;
	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n:%v%&");
		CoreLogger = spdlog::stdout_color_mt("Engine");
		ClientLogger = spdlog::stdout_color_mt("App");
		CoreLogger->set_level(spdlog::level::trace);
		ClientLogger->set_level(spdlog::level::trace);
	}
}
