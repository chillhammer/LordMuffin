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
#include <GameObject/GameObjectHelpers.h>
#include <Camera/CameraComponent.h>
#include "GameStates.h"
/**
	Each state runs code that determines the course of the game.
	Each state has local variables which can be seen in GameManager.h
**/
namespace Skel::GameStates
{
	// Test State
	void TestEntity::Enter(GameManager* owner)
	{
		m_Light = Vector3(0.f, 10.f, -5.f);

		m_Shader = Resources.GetShader("Model");
		m_Shader->Bind();
		m_Shader->SetUniform3f("u_LightPosition", m_Light.x, m_Light.y, m_Light.z);

		m_SkinnedShader = Resources.GetShader("SkinnedModel");
		m_SkinnedShader->Bind();
		m_SkinnedShader->SetUniform3f("u_LightPosition", m_Light.x, m_Light.y, m_Light.z);
		
		Game.LoadScene("TestLevel");
	}
	void TestEntity::Execute(GameManager* owner)
	{
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
			if (Input.IsKeyPressed(KEY_R))
			{
				Client.SetConnected(false);
			}
		}
		ImGui::End();

		if (Input.IsKeyPressed(KEY_T))
		{
			GameObject* bendy = Objects::FindObjectByName("Bendy");
			if (bendy)
			{
				bendy->Destroy();
			}
		}
	}
	void TestEntity::Exit(GameManager* owner)
	{
	}
}