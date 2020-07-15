#include "SkelPCH.h"
#include "ServerManager.h"
#include <Net/Socket.h>
#include <Net/Buffer.h>
#include <thread>
#include <Resources/ResourceManager.h>
#include <Objects/Player/PlayerObject.h>
#include <FakeLag/FakeLagPacketHolderManager.h>

#include <Packets/PlayerInputPacket.h>
#include <Packets/JoinPackets.h>
#include <Packets/SnapshotPacket.h>
#include <Packets/SyncPackets.h>
#include <Net/Packet.h>
#include <limits>

namespace Skel::Net {

	ServerManager& ServerManager::Instance()
	{
#ifndef SERVER
		ASSERT(false, "Cannot reference Server singleton from Client");
#endif
		static ServerManager instance;
		return instance;
	}


	// Server Main Function!
	void ServerManager::ServerMain()
	{	

		m_Server.Bind(Net::GetServerAddress().GetPort());

		const double startTime = Server.RunningTime();

		double secondMarker = startTime;

		double previousFrameTime = startTime;
		double targetFrameTime = startTime + Server.GetFixedFrameDeltaTime();
		int noSleepTicks = 0;

		Game.LoadScene("TestLevel");

		while (Server.IsRunning())
		{
			const double startFrameTime = Server.RunningTime();

			// Run all objects in the scene
			for (GameObject* o : Game.Objects())
			{
				o->UpdateComponents();
			}
			for (GameObject* o : Game.Objects())
			{
				o->PostUpdateComponents();
			}
			

#pragma region Fixed Framerate Loop
			double endFrameTime = Server.RunningTime();

			// Find out how many dropped frames. 
			
			// If ended time above target (dropped)
			if (endFrameTime > targetFrameTime)
			{
				//targetFrameTime += Server.GetFixedFrameDeltaTime(); disable temp
				noSleepTicks++;
			} else if (noSleepTicks > 0) {
				LOG("Sped up Ticks: {0}  | Time: {1}", noSleepTicks, endFrameTime-startFrameTime);
				noSleepTicks = 0;
			}

			// Sleep until next frame
			Server.Sleep(std::max(0.0, targetFrameTime - Server.RunningTime() - (2.25 * 0.001)));

			// Reset Target Variables
			previousFrameTime = targetFrameTime;
			targetFrameTime += Server.GetFixedFrameDeltaTime();

			// Debug Ticks Per Second
			/*if (Server.RunningTime() >= secondMarker + 1) {
				secondMarker++;
				LOG("Tick {0} at second: {1}", m_Tick, secondMarker - startTime);
			}*/

			++m_Tick;

			
#pragma endregion
		}
	}

	bool ServerManager::IsRunning() const
	{
		return m_Running;
	}

	uint64 ServerManager::GetTick() const
	{
		return m_Tick;
	}

	double ServerManager::RunningTime() const
	{
		uint64_t value;
		QueryPerformanceCounter((LARGE_INTEGER*)& value);

		return double(value) / 10000000;
		//return double(std::chrono::high_resolution_clock::now().time_since_epoch().count()) / 1000000000.0;
	}

	void ServerManager::Sleep(double time) const
	{
		std::this_thread::sleep_for(std::chrono::milliseconds((int64_t)(time * 1000.0f)));
	}

}