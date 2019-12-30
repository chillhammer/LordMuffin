#include "SkelPCH.h"
#include "ServerManager.h"
#include <Net/Socket.h>
#include <Net/Buffer.h>
#include <thread>
#include <Resources/ResourceManager.h>
#include <Objects/Player/PlayerObject.h>

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
		// Setting Up Dependencies
		Net::Init();

		Net::Socket server;
		server.Bind(Net::GetServerAddress().GetPort());

		Skel::PlayerObject playerObjs[MAX_PLAYERS];
		m_ClientHandler.SetPlayerObjectArray(&(*playerObjs));

		const double startTime = Server.RunningTime();

		double previousFrameTime = startTime;
		double targetFrameTime = startTime + Server.GetFixedFrameDeltaTime();

		Net::Buffer buffer;
		Net::Address fromAddress;

		while (Server.IsRunning())
		{
			const double startFrameTime = Server.RunningTime();

			LOG("Start: {0}", startFrameTime- startTime);


			// Receive Packets
			int packetsReceived = 0;
			while (server.ReceiveBuffer(buffer, fromAddress)) {
				packetsReceived++;

				// Find out what packet is being received
				PacketType packetType;
				buffer.ResetReadPosition();
				buffer.Read(&packetType, 1);

				switch (packetType) {
				case PACKET_JOIN_REQUEST:
				{
					JoinRequestPacket packet;
					packet.ReadFromBuffer(buffer);

					if (m_ClientHandler.RemainingSlots() > 0) 
					{
						auto clientID = m_ClientHandler.AddPlayer(fromAddress);
						WRITE_PACKET(JoinAcceptPacket, (clientID), buffer);
						server.SendBuffer(buffer, fromAddress);
					}
					else // Reject
					{
						WRITE_PACKET(JoinDeclinedPacket, , buffer);
						server.SendBuffer(buffer, fromAddress);
					}

					
				}
					break;

				case PACKET_SYNC_REQUEST:
				{
					READ_PACKET(SyncRequestPacket, buffer); // creates packet
					{
						WRITE_PACKET(SyncServerTimePacket, (packet.SyncID, RunningTime()), buffer); // sends back server time
						server.SendBuffer(buffer, fromAddress);
					}
				}
					break;
				case PACKET_INPUT:
				{
					PlayerInputPacket packet;
					packet.ReadFromBuffer(buffer);
					if (m_ClientHandler.IsActive(packet.ClientID)) {
						PlayerInputState input = packet.InputState;
						PlayerObject& obj = playerObjs[packet.ClientID];

						obj.ProcessInput(input, GetFixedFrameDeltaTime());
					}
					else
					{
						LOG_WARN("Receiving Input from inactive client. ID: {0}", packet.ClientID);
					}
					break;
				}
				}
			}
			//LOG("Packets Received: {0}", packetsReceived);


			// Creates Snapshot
			WRITE_PACKET(PlayerSnapshotPacket, (m_ClientHandler), buffer); // Takes snapshot of all player objects

			// Broadcast to Active Clients
			const std::vector<ClientSlot>& clientSlots = m_ClientHandler.GetClientSlots();
			for (const ClientSlot& slot : clientSlots) {
				server.SendBuffer(buffer, slot.ClientAddress);
			}


			++m_Tick;

#pragma region Fixed Framerate Loop
			double endFrameTime = Server.RunningTime();
			//LOG("Delta time this frame: {0} - Fixed rate: {1}", endFrameTime - startFrameTime, Server.GetFixedFrameDeltaTime());

			// Find out how many dropped frames. 
			int droppedFrames = 0;
			// If ended time far from target, then ignore those frames (dropped)
			while (endFrameTime > targetFrameTime + 0.5 * Server.GetFixedFrameDeltaTime())
			{
				LOG("End Frame Time went past target. End: {0}\tTarget: {1}", endFrameTime - startTime, targetFrameTime-startTime);
				targetFrameTime += Server.GetFixedFrameDeltaTime();
				droppedFrames++;
				endFrameTime = Server.RunningTime();
			}
			if (droppedFrames > 0) {
				LOG("Dropped frames: {0}  | Time: {1}", droppedFrames, endFrameTime-startFrameTime);
			}

			// Sleep until next frame
			//ASSERT(nextFrameTime >= endFrameTime, "Dropped frames not handled appropriately");
			Server.Sleep(std::max(0.0, targetFrameTime - Server.RunningTime()));

			// Checking actual delta
			//double actualDelta = Server.RunningTime() - startFrameTime;
			//LOG("Actual delta: {0}\tDelta: {1}", actualDelta, endFrameTime - startFrameTime);

			// Reset Target Variables
			previousFrameTime = targetFrameTime;
			targetFrameTime += Server.GetFixedFrameDeltaTime();

			if (Server.RunningTime() > targetFrameTime) {
				//LOG("end running time > target. end: {0}\ttarget: {1}", Server.RunningTime() - startTime, targetFrameTime - startTime);
			}
			LOG("End: {0}", Server.RunningTime() - startTime);
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