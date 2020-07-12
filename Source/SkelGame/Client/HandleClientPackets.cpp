#include "SkelPCH.h"
#include <Net/Net.h>
#include <Net/Packet.h>
#include <Packets/JoinPackets.h>
#include <Server/ServerManager.h>
#include <Game/GameManager.h>
#include <FakeLag/FakeLagPacketHolderManager.h>
#include <Objects/Network/NetworkComponent.h>
#include <Camera/CameraComponent.h>
#include <GameObject/GameObjectHelpers.h>
#include <Resources/ResourceManager.h>
#include <Input/InputManager.h>
namespace Skel
{
	using namespace Net;

	void NetworkComponent::HandleClientPackets()
	{
		Buffer buffer;
		Address fromAddress;
		CameraComponent& camera = Objects::FindFirstComponent<CameraComponent>();

		// Connecting to Server!
		if (!Client.Connected()) 
		{
			// Client Receive Join Accept Or Decline
			Net::Buffer receiveBuffer;
			while (Client.ReceiveBuffer(receiveBuffer)) {

				Net::PacketType type;
				receiveBuffer.ResetReadPosition();
				receiveBuffer.Read(&type, 1);

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


		if (Client.Connected())
		{
			// Client Send Inputs
			if (!Client.IsSynchronizing())
			{
				Net::Buffer buffer;

				PlayerComponent* playerComp = GetPlayerComponent(Client.GetClientID());

				// Send Input If Player Object exists
				if (playerComp)
				{
					playerComp->GetOwner()->ObjectTransform.SetYaw(camera.GetOwner()->ObjectTransform.GetYaw());

					PlayerInputState input = { Input.IsKeyDown(KEY_W), Input.IsKeyDown(KEY_S), Input.IsKeyDown(KEY_D),
						Input.IsKeyDown(KEY_A), Input.IsKeyPressed(KEY_SPACE), playerComp->GetOwner()->ObjectTransform.GetYaw() };
					if (camera.Mode == CameraMode::NoClip) {
						input = PlayerInputState();
						input.Yaw = playerComp->GetOwner()->ObjectTransform.GetYaw();
					}

					Net::PlayerInputPacket packet(input, Client.GetClientID(), Game.GetTick(), Game.DeltaTimeUnscaled());

					// Add to lag simulator
					FakeLagPackets.AddPacket<Net::PlayerInputPacket>(packet);

					// Pull from lag simulator
					FakeLagPackets.PopAndSendToServer<Net::PlayerInputPacket>(buffer);

					// Client-predict for player
					playerComp->ProcessInput(input, Game.DeltaTimeUnscaled());
					//playerObj->ProcessAnimation(input);

					// Record personal input & state so that we can rollback
					Client.GetPredictionHistory().RecordState(input, PlayerSnapshotState(*playerComp));
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
			Client.GetSnapshotReceiver().Update(this);

		}
	}
}