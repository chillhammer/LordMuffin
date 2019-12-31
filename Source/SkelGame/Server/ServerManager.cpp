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

		double secondMarker = startTime;

		double previousFrameTime = startTime;
		double targetFrameTime = startTime + Server.GetFixedFrameDeltaTime();
		int noSleepTicks = 0;

		Net::Buffer buffer;
		Net::Address fromAddress;

		while (Server.IsRunning())
		{
			const double startFrameTime = Server.RunningTime();

			//LOG("Start: {0}", startFrameTime- startTime);
			
			

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