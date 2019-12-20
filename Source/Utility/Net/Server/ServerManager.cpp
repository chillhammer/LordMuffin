#include "SkelPCH.h"
#include "ServerManager.h"
#include <Net/Socket.h>
#include <Net/Buffer.h>
#include <thread>
#include <Objects/Player/PlayerInputState.h>
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
	// Setting Up Dependencies
	Log::Init();
	Net::Init();

	Net::Socket server;
	server.Bind(Net::GetServerAddress().GetPort());

	const double startTime = Server.RunningTime();

	double previousFrameTime = startTime;
	double targetFrameTime = previousFrameTime + Server.GetFixedFrameDeltaTime();

	while (Server.IsRunning())
	{
		const double startFrameTime = Server.RunningTime();


		Net::Buffer buffer;
		Net::Address fromAddress;
		// Send back
		if (server.ReceiveBuffer(buffer, fromAddress)) {
			server.SendBuffer(buffer, fromAddress);

			PlayerInputState input;
			memcpy(&input, buffer.Data(), buffer.Length());
			LOG("Got buffer, state= ({0},{1})", input.Forward, input.Back);
		}


		#pragma region Fixed Framerate Loop
		const double endFrameTime = Server.RunningTime();
		LOG("Delta time this frame: {0} - Fixed rate: {1}", endFrameTime - startFrameTime, Server.GetFixedFrameDeltaTime());

		// Find out how many dropped frames. 
		int droppedFrames = 0;
		// If ended time far from target, then ignore those frames (dropped)
		while (endFrameTime > targetFrameTime + 0.5 * Server.GetFixedFrameDeltaTime())
		{
			targetFrameTime += Server.GetFixedFrameDeltaTime();
			droppedFrames++;
		}
		if (droppedFrames > 0) {
			LOG("Dropped frames: {0}", droppedFrames);
		}

		// Sleep until next frame
		//ASSERT(nextFrameTime >= endFrameTime, "Dropped frames not handled appropriately");
		Server.Sleep(std::max(0.0, targetFrameTime - endFrameTime));

		// Checking actual delta
		double actualDelta = Server.RunningTime() - startFrameTime;
		LOG("Actual delta: {0}", actualDelta);

		// Reset Target Variables
		previousFrameTime = targetFrameTime;
		targetFrameTime += Server.GetFixedFrameDeltaTime();
		#pragma endregion
	}
}

bool ServerManager::IsRunning() const
{
	return m_Running;
}

double ServerManager::RunningTime() const
{
	return double(std::chrono::high_resolution_clock::now().time_since_epoch().count()) / 1000000000.0;
}

void ServerManager::Sleep(double time) const
{
	std::this_thread::sleep_for(std::chrono::milliseconds((int64_t)(time * 1000.0f)));
}

}