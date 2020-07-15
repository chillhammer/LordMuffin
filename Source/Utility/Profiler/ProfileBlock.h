#pragma once
#include <chrono>
#include <Logger/Log.h>
#include <string_view>

namespace Skel
{
	class ProfilerBlock
	{
	public:
		ProfilerBlock(const char* name, bool condition = true) : m_Name(name) {
			Start(condition);
		};
		void Start(bool condition) {
			m_Active = condition;
			m_StartTime = Clock::now();
		};
		void Start() {
			Start(true);
		};
		void End() {
			m_EndTime = Clock::now();
			if (m_Active)
			{
				m_Active = false;
				std::chrono::duration<float, std::milli> dur = m_EndTime - m_StartTime;
				LOG("Timer [{0}]: {1} ms", m_Name, dur.count());
			}
		}

		float RecordTime() {
			End();
			return static_cast<float>((m_EndTime - m_StartTime).count());
		}

		void SetActive(bool active) { m_Active = active; }
		~ProfilerBlock() {
			End();
		};
	private:
		ProfilerBlock() {};
		using Clock = std::chrono::high_resolution_clock;
		using Time = std::chrono::time_point<Clock>;

		bool m_Active;
		Time m_StartTime;
		Time m_EndTime;
		const char* m_Name;
	};
}