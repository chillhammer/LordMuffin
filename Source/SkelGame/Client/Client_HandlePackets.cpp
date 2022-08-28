#include "SkelPCH.h"
#include <Net/Net.h>
#include <Net/Packet.h>
#include <Packets/JoinPackets.h>
#include <Server/ServerManager.h>
#include <Game/GameManager.h>
#include <GameObject/GameObjectManager.h>
#include <FakeLag/FakeLagPacketHolderManager.h>
#include <Objects/Network/NetworkComponent.h>
#include <Camera/CameraComponent.h>
#include <Resources/ResourceManager.h>
#include <Input/InputManager.h>
namespace Skel
{
	using namespace Net;

	void NetworkComponent::Client_HandlePackets()
	{
		Buffer buffer;
		Address fromAddress;

		// Connecting to Server!
		if (!Client.Connected()) 
		{
			// Client Receive Join Accept Or Decline
			Net::Buffer receiveBuffer;
			while (Client.ReceiveBuffer(receiveBuffer)) {

				Net::PacketType type;
				receiveBuffer.ResetReadPosition();
				receiveBuffer.Read(&type, 1);
				Client.SetLastReceivedTime(RUNNING_TIME);

				switch (type) {
				case Net::PACKET_JOIN_ACCEPT:
				{
					LOG("Connected to Server");
					READ_PACKET(Net::JoinAcceptPacket, receiveBuffer);
					Client.SetClientID(packet.ClientID);
					Client.SetConnected(true);
					Client.GetSynchronizer().StartSynchronizing();
					break;
				}
				case Net::PACKET_JOIN_DECLINED:
					LOG("Packet Join Declined");
					break;

				case Net::PACKET_INPUT:
				{
					//Net::PlayerInputPacket packet;
					//packet.ReadFromBuffer(receiveBuffer);
					ASSERT(false, "Client should not receive input packet");
					break;
				}
				}
			}
			

		}
		///////////////////////////

		// Move local player
		PlayerInputState input;
		CameraComponent& camera = Objects.FindFirstComponent<CameraComponent>();
		if (m_LocalPlayer)
		{
			PlayerComponent playerComp = m_LocalPlayer->GetComponent<PlayerComponent>();
			// Rotate player to align with camera
			m_LocalPlayer->ObjectTransform.SetYaw(camera.GetOwner()->ObjectTransform.GetYaw());
			// Create input
			input = { Input.IsKeyDown(KEY_W), Input.IsKeyDown(KEY_S), Input.IsKeyDown(KEY_D),
						Input.IsKeyDown(KEY_A), Input.IsKeyPressed(KEY_SPACE), m_LocalPlayer->ObjectTransform.GetYaw() };

			// Client-side predict
			playerComp.ProcessInput(input, static_cast<float>(Game.DeltaTimeUnscaled()));

		}


		if (Client.Connected())
		{
			// Client Send Inputs
			if (!Client.IsSynchronizing())
			{	
				// Send Input
				if (m_LocalPlayer)
				{
					PlayerComponent& playerComp = m_LocalPlayer->GetComponent<PlayerComponent>();

					// Don't move in NoClip
					if (camera.Mode == CameraMode::NoClip) {
						input = PlayerInputState();
						input.Yaw = m_LocalPlayer->ObjectTransform.GetYaw();
					}

					Net::PlayerInputPacket packet(input, Client.GetClientID(), Game.GetTick(), static_cast<float>(Game.DeltaTimeUnscaled()));

					// Add to lag simulator
					FakeLagPackets.AddPacket<Net::PlayerInputPacket>(packet);

					// Pull from lag simulator
					FakeLagPackets.PopAndSendToServer<Net::PlayerInputPacket>(buffer);

					// Record personal input & state so that we can rollback
					Client.GetPredictionHistory().RecordState(input, PlayerSnapshotState(playerComp, false));
				}
			}
			else
				// Client Synchronizing
			{
				Client.GetSynchronizer().UpdateUntilSynchronized();
			}

			// Client Receive Snapshots and Sync Packets
			Net::Buffer receiveBuffer;
			PlayerInputState input;
			while (Client.ReceiveBuffer(receiveBuffer)) {
				Net::PacketType type;
				receiveBuffer.ResetReadPosition();
				receiveBuffer.Read(&type, 1);
				Client.SetLastReceivedTime(RUNNING_TIME);

				switch (type)
				{
				case Net::PACKET_SYNC_SERVER_TIME:
				{
					READ_PACKET(Net::SyncServerTimePacket, receiveBuffer); // creates packet object
					Client.GetSynchronizer().ReceiveServerTimePacket(packet);
					break;
				}
				case Net::PACKET_SNAPSHOT:
				{
					READ_PACKET(Net::PlayerSnapshotPacket, receiveBuffer); // creates packet object
					Client.GetSnapshotReceiver().ReceiveSnapshotPacket(packet);
					break;
				}

				}

			}
			Client.GetSnapshotReceiver().Update();

			// Assertions
			ASSERT(m_LocalPlayer == m_PlayerObjects[Client.GetClientID()], "Local player object must be the same in array");

			// Timeout
			if (ENABLE_CLIENT_TIMEOUT && RUNNING_TIME - Client.GetLastReceivedTime() > Net::CLIENT_TIMEOUT_TIME)
			{
				Client.SetConnected(false);
				LOG("Timeout. Disconnect from server");
			}
		}
	}
}