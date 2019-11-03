#pragma once
#include "spdlog/spdlog.h"

namespace Skel {

	template class std::shared_ptr<spdlog::logger>;

	class Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }
	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};

	//Core
#define LOG(...)			::Skel::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)		::Skel::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)		::Skel::Log::GetLogger()->error(__VA_ARGS__)

}
