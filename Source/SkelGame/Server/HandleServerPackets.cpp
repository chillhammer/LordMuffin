#include "SkelPCH.h"
#include <Net/Net.h>
#include <Net/Packet.h>
#include <Packets/JoinPackets.h>
#include <Server/ServerManager.h>
#include <Game/GameManager.h>
#include <FakeLag/FakeLagPacketHolderManager.h>
#include <Objects/Network/NetworkComponent.h>
namespace Skel
{
	using namespace Net;

	void NetworkComponent::HandleServerPackets()
	{
		Buffer buffer;
		Address fromAddress;
		Socket& m_Server = Server.GetSocket();
		ClientHandler& m_ClientHandler = Server.GetClientHandler();

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

				if (m_ClientHandler.RemainingSlots() > 0)
				{
					auto clientID = m_ClientHandler.AddPlayer(fromAddress);
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
				if (m_ClientHandler.IsActive(packet.ClientID)) {
					PlayerInputState input = packet.InputState;
					PlayerObject& obj = playerObjs[packet.ClientID];

					// Update latest client tick, will also ack and process input
					if (packet.ClientTick > m_ClientHandler.GetClientTick(packet.ClientID)) {
						m_ClientHandler.UpdateClientTick(packet.ClientID, packet.ClientTick);
						obj.ProcessInput(input, packet.DeltaTime);
						obj.ProcessAnimation(input);
					}
					// Process Old Input (stale tick)
					else if (m_ClientHandler.TryInputAck(packet.ClientID, packet.ClientTick)) {
						obj.ProcessInput(input, packet.DeltaTime);
						//if (packet.InputState.Jump) LOG("Jumping from Stale Tick Input");
					}

					// Check Recent Previous Inputs for un-acked states
					for (int i = 0; i < Net::INPUTS_PACKED; ++i) {
						uint64 oldTick = packet.ClientTick - 1 - i;
						if (m_ClientHandler.TryInputAck(packet.ClientID, oldTick)) {
							// Processing Old Inputs using new input delta time
							// This is bad, but I think it's better than having to do N delta times (8 bytes each)
							obj.ProcessInput(packet.RecentInputs[i], packet.DeltaTime);
							//if (packet.RecentInputs[i].Jump) 
								//LOG("Jumping from Input Ack");
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
				m_ClientHandler.RemovePlayer(m_ClientHandler.GetClientIndex(fromAddress));
				break;
			}
		}
		//LOG("Packets Received: {0}", packetsReceived);

		m_TimeSinceSnapshotSent += GetFixedFrameDeltaTime();

		if (m_TimeSinceSnapshotSent >= SNAPSHOT_RATE)
		{
			m_TimeSinceSnapshotSent = 0.0;


			// Broadcast to Active Clients
			const std::vector<ClientSlot>& clientSlots = m_ClientHandler.GetClientSlots();
			for (const ClientSlot& slot : clientSlots) {
				// Creates Snapshot
				WRITE_PACKET(PlayerSnapshotPacket, (m_ClientHandler, slot.ID), buffer); // Takes snapshot of all player objects


				FakeLagPackets.AddPacket(packet, slot.ClientAddress);
				//m_Server.SendBuffer(buffer, slot.ClientAddress);
			}

		}

		FakeLagPackets.PopAndSendToRecipient<PlayerSnapshotPacket>(buffer);
		FakeLagPackets.PopAndSendToRecipient<SyncServerTimePacket>(buffer);
	}
}