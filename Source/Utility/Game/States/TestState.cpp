#include <skelpch.h>
#include <Input/InputManager.h>
#include <Resources/ResourceManager.h>
#include <Packets/PlayerInputPacket.h>
#include <Packets/JoinPackets.h>
#include <Packets/SnapshotPacket.h>
#include <Client/ClientManager.h>
#include <FakeLag/FakeLagPacketHolderManager.h>
#include <Net/Net.h>
#include <imgui.h>
#include <Graphics/Model/ImportedSkinnedModel.h>
#include "GameStates.h"
/**
	Each state runs code that determines the course of the game.
	Each state has local variables which can be seen in GameManager.h
**/
namespace Skel::GameStates
{
	// Test State
	void Test::Enter(GameManager* owner)
	{
		LOG("Entering Test State in Game Object");
		m_Camera.Init();
		m_Light = Vector3(0.f, 10.f, -5.f);

		m_Shader = Resources.GetShader("Model");
		m_Shader->Bind();
		m_Shader->SetUniform3f("u_LightPosition", m_Light.x, m_Light.y, m_Light.z);
		m_SkinnedShader = Resources.GetShader("SkinnedModel");
		m_SkinnedShader->Bind();
		m_SkinnedShader->SetUniform3f("u_LightPosition", m_Light.x, m_Light.y, m_Light.z);

		m_DebugShader = Resources.GetShader("Debug");

		m_Box.ObjectTransform.Scale = Vector3(15, 0.001f, 30);

		m_Player.ObjectTransform.Position = Vector3(2.0f, 1.0f, 0.0f);

		for (int i = 0; i < Net::MAX_PLAYERS; ++i)
		{
			PlayerObject& obj = m_PlayerObjectArray[i];
			obj.ObjectTransform.Position.x = i;
			obj.ObjectTransform.Position.y = 1;
		}

	}
	void Test::Execute(GameManager* owner)
	{
		ProfilerBlock fpsCounter("FPS", false);
		m_Camera.Update();

		// FPS
		ImGui::Begin("FPS");
		ImGui::Text("MS: %f", m_FrameTime * 1e-6);
		ImGui::End();

		// ImGui Connection Window
		ImGui::Begin("Network Connection Window");
		if (!Client.Connected()) {
			if (ImGui::Button("Connect!") || Input.IsKeyPressed(KEY_E)) {
				Net::Buffer buffer;
				Net::JoinRequestPacket packet;
				packet.WriteToBuffer(buffer);
				Client.SendBuffer(buffer);
			}
			ImGui::SliderFloat("Fake Lag (s)", &Net::FAKE_LAG_S, 0.0f, 1.0f, "ratio = %.3f");
			ImGui::SliderFloat("Fake Jitter (s)", &Net::FAKE_JITTER_S, 0.0f, 0.5f, "ratio = %.3f");
			ImGui::SliderFloat("Fake Packet Loss (%)", &Net::FAKE_PACKET_LOSS, 0.0f, 1.0f, "ratio = %.3f");
		}
		else {
			ImGui::Text("Connected");
			ImGui::Text("Fake Lag (%f), Jitter(%f), Packet Loss(%f)", Net::FAKE_LAG_S, Net::FAKE_JITTER_S, Net::FAKE_PACKET_LOSS);
		}
		ImGui::End();

		// Draw Bendy
		/*ImportedSkinnedModel* bendy = (ImportedSkinnedModel*) &(*Resources.GetModel("Bendy"));
		bendy->UpdateBoneTransforms(Game.RunningTime());
		bendy->UpdateBoneShader(Resources.GetShader("SkinnedModel"));
		bendy->Draw(Resources.GetShader("SkinnedModel"), Matrix4x4(0.5f));*/
		//ModelPtr bendy = Resources.GetModel("Bendy");


		//ImportedSkinnedModel* bendy = (ImportedSkinnedModel*) & (*Resources.GetModel("Soldier"));
		ImportedSkinnedModel* bendy = (ImportedSkinnedModel*) & (*Resources.GetModel("Bendy"));
		if (Input.IsKeyDown(KEY_T)) {
			bendy->SetAnimationIndex(1);
		}
		else {
			bendy->SetAnimationIndex(0);
		}
		bendy->UpdateBoneTransforms(Game.RunningTime());
		bendy->UpdateBoneShader(Resources.GetShader("SkinnedModel"));
		Matrix4x4 bendyMatrix = Matrix4x4(1.0f);

		Matrix4x4 posMat = glm::translate(posMat, Vector3(1.0f, 0.0f, 0.0f));

		bendyMatrix *= posMat;

		//bendy->Draw(Resources.GetShader("SkinnedModel"), bendyMatrix);

		PlayerObject p;
		p.ObjectTransform.Position = Vector3(-1.0f, 0.0f, -1.0f);
		p.ObjectTransform.SetYaw(180.0f);
		p.Draw(m_SkinnedShader);

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

				PlayerObject& playerObj = m_PlayerObjectArray[Client.GetClientID()];
				playerObj.ObjectTransform.SetYaw(m_Camera.ObjectTransform.GetYaw());

				PlayerInputState input = { Input.IsKeyDown(KEY_W), Input.IsKeyDown(KEY_S), Input.IsKeyDown(KEY_D), 
					Input.IsKeyDown(KEY_A), Input.IsKeyPressed(KEY_SPACE), playerObj.ObjectTransform.GetYaw() };
				if (m_Camera.Mode == CameraMode::NoClip) {
					input = PlayerInputState();
					input.Yaw = playerObj.ObjectTransform.GetYaw();
				}

				Net::PlayerInputPacket packet(input, Client.GetClientID(), Game.GetTick(), Game.DeltaTimeUnscaled());

				// Add to lag simulator
				FakeLagPackets.AddPacket<Net::PlayerInputPacket>(packet);

				// Pull from lag simulator
				FakeLagPackets.PopAndSendToServer<Net::PlayerInputPacket>(buffer);
				
				// Client-predict for player
				playerObj.ProcessInput(input, Game.DeltaTimeUnscaled());
				playerObj.ProcessAnimation(input);

				// Record personal input & state so that we can rollback
				Client.GetPredictionHistory().RecordState(input, PlayerSnapshotState(playerObj));
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
			Client.GetSnapshotReceiver().Update();

		}
		// Update Camera
		m_Camera.SetPivotPosition(m_PlayerObjectArray[Client.GetClientID()].ObjectTransform.Position);

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_ViewProjection", m_Camera.GetProjectionMatrix()* m_Camera.GetViewMatrix());
		m_SkinnedShader->Bind();
		m_SkinnedShader->SetUniformMat4f("u_ViewProjection", m_Camera.GetProjectionMatrix()* m_Camera.GetViewMatrix());
		m_DebugShader->Bind();
		m_DebugShader->SetUniformMat4f("u_ViewProjection", m_Camera.GetProjectionMatrix()* m_Camera.GetViewMatrix());

		m_Box.Draw();

		//m_Player.Draw();
		const auto& activePlayers = Client.GetSnapshotReceiver().GetActiveClients();
		for (uint16 clientID : activePlayers) {
			m_PlayerObjectArray[clientID].Draw(m_SkinnedShader);
		}

		// Record Frame Time
		m_FrameTime = fpsCounter.RecordTime();
		
	}
	void Test::Exit(GameManager* owner)
	{
	}
}