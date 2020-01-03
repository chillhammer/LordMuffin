#pragma once
#include "ClientHandler.h"
#include <Net/Socket.h>

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

		uint64 GetTick() const;
		double RunningTime() const;
		void Sleep(double time) const;

		double GetFixedFrameDeltaTime() const { return 1.0 / 60.0; };
		Socket& GetSocket() { return m_Server; }

		~ServerManager() {};
	private:
		ServerManager() {};

		// Properties
		ClientHandler m_ClientHandler;
		uint64 m_Tick = 0;
		double m_TimeSinceSnapshotSent = 0;
		bool m_Running = true;
		Socket m_Server; // server socket
	};
}