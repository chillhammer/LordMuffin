#include "SkelPCH.h"
#include <Net/Net.h>
#include <Net/Packet.h>
#include <Packets/JoinPackets.h>
#include <Server/ServerManager.h>
#include <Game/GameManager.h>
#include <FakeLag/FakeLagPacketHolderManager.h>
#include <Objects/Network/NetworkComponent.h>
#include <GameObject/GameObjectTemplate.h>
#include <Resources/ResourceManager.h>
namespace Skel
{
	using namespace Net;

	void NetworkComponent::Server_HandlePackets()
	{
		Buffer buffer;
		Address fromAddress;
		Socket& m_Server = Server.GetSocket();
		ClientHandler& clientHandler = Server.GetClientHandler();

		// Receive Packets
		int packetsReceived = 0;
		while (m_Server.ReceiveBuffer(buffer, fromAddress)) {
			packetsReceived++;

			// Find out what packet is being received
			PacketType packetType;
			buffer.ResetReadPosition();
			buffer.Read(&packetType, 1);

			switch (packetType) {

			//////////////////////
			// Packet Join Request
			// Player wants to join server
			case PACKET_JOIN_REQUEST:
			{
				JoinRequestPacket packet;
				packet.ReadFromBuffer(buffer);

				if (clientHandler.RemainingSlots() > 0)
				{
					uint16 clientID = 0;
					if (clientHandler.ClientExists(fromAddress))
					{
						clientID = clientHandler.GetClientIndex(fromAddress);
					}
					else
					{
						clientID = clientHandler.AddPlayer(fromAddress);
					}

					WRITE_PACKET(JoinAcceptPacket, (clientID), buffer);
					m_Server.SendBuffer(buffer, fromAddress);
				}
				else // Reject
				{
					WRITE_PACKET(JoinDeclinedPacket, , buffer);
					m_Server.SendBuffer(buffer, fromAddress);
				}


			}
			break;

			//////////////////////
			// Sync Request Packet
			// Player wants to sync with server
			case PACKET_SYNC_REQUEST:
			{
				READ_PACKET(SyncRequestPacket, buffer); // creates packet
				{
					WRITE_PACKET(SyncServerTimePacket, (packet.SyncID, Server.RunningTime()), buffer); // sends back server time
					FakeLagPackets.AddPacket(packet, fromAddress);
					//m_Server.SendBuffer(buffer, fromAddress);
				}
			}
			break;

			/////////////////////
			// Player Input Packet
			// Player is trying to move themself
			case PACKET_INPUT:
			{
				READ_PACKET(PlayerInputPacket, buffer);
				if (clientHandler.IsActive(packet.ClientID)) {
					PlayerInputState input = packet.InputState;
					PlayerComponent* obj = GetPlayerComponent(packet.ClientID);

					// Update latest client tick, will also ack and process input
					if (packet.ClientTick > clientHandler.GetClientTick(packet.ClientID)) {
						clientHandler.UpdateClientTick(packet.ClientID, packet.ClientTick);
						obj->ProcessInput(input, packet.DeltaTime);
						//obj->ProcessAnimation(input);
					}
					// Process Old Input (stale tick)
					else if (clientHandler.TryInputAck(packet.ClientID, packet.ClientTick)) {
						obj->ProcessInput(input, packet.DeltaTime);
						//if (packet.InputState.Jump) LOG("Jumping from Stale Tick Input");
					}

					// Check Recent Previous Inputs for un-acked states
					for (int i = 0; i < Net::INPUTS_PACKED; ++i) {
						uint64 oldTick = packet.ClientTick - 1 - i;
						if (clientHandler.TryInputAck(packet.ClientID, oldTick)) {
							// Processing Old Inputs using new input delta time
							// This is bad, but I think it's better than having to do N delta times (8 bytes each)
							obj->ProcessInput(packet.RecentInputs[i], packet.DeltaTime);
						}
					}
				}
				else
				{
					LOG_WARN("Receiving Input from inactive client. ID: {0}", packet.ClientID);
				}
				break;
			}
			// Remove player if they leave
			case PACKET_QUIT:
				clientHandler.RemovePlayer(clientHandler.GetClientIndex(fromAddress));
				break;
			}
		}
		//LOG("Packets Received: {0}", packetsReceived);

		m_TimeSinceSnapshotSent += Server.GetFixedFrameDeltaTime();

		if (m_TimeSinceSnapshotSent >= SNAPSHOT_RATE)
		{
			m_TimeSinceSnapshotSent = 0.0;


			// Broadcast to Active Clients
			const std::vector<ClientSlot>& clientSlots = clientHandler.GetClientSlots();
			for (const ClientSlot& slot : clientSlots) {
				// Creates Snapshot
				WRITE_PACKET(PlayerSnapshotPacket, (clientHandler, slot.ID), buffer); // Takes snapshot of all player objects


				FakeLagPackets.AddPacket(packet, slot.ClientAddress);
				//m_Server.SendBuffer(buffer, slot.ClientAddress);
			}

		}

		FakeLagPackets.PopAndSendToRecipient<PlayerSnapshotPacket>(buffer);
		FakeLagPackets.PopAndSendToRecipient<SyncServerTimePacket>(buffer);

		// Timeout per server
		auto& clients = clientHandler.GetClientSlots();
		if (ENABLE_SERVER_TIMEOUT_PER_CLIENT) 
		{
			for (auto& client : clients)
			{
				uint64 tickTimeout = Net::SERVER_TIMEOUT_TIME_PER_CLIENT * Net::SERVER_TICK_PER_SEC;
				uint64 lastReceived = clientHandler.GetClientLastReceivedTick(client.ID);
				if (Server.GetTick() - lastReceived > tickTimeout && lastReceived > 0)
				{
					clientHandler.RemovePlayer(client.ID);
					LOG("Timeout. Disconnecting client: {0}", client.ID);
				}
			}
		}
	}
}