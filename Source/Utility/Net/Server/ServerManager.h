#pragma once

#define Server Net::ServerManager::Instance()
namespace Skel::Net
{
	/**
		Manager singleton object handling the server logic.
		Can only be used by the server. Replaces GameManager
	**/
	class ServerManager
	{
	public:
		static ServerManager& Instance();
		void ServerMain();
		bool IsRunning() const;

		double RunningTime() const;
		void Sleep(double time) const;

		double GetFixedFrameDeltaTime() const { return 1.0 / 60.0; }

		~ServerManager() {};
	private:
		ServerManager() {};
		//void UpdateDeltaTime();

		// Properties
		double m_LastUpdatedTime = 0;
		double m_DeltaTime = 0;
		bool m_Running = true;
		bool m_Paused = false;
	};
}