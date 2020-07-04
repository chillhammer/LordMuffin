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

		///////////////////////////
		// Update Camera
		// m_Camera.SetPivotPosition(m_PlayerObjectArray[Client.GetClientID()].ObjectTransform.Position);
	}
	void TestEntity::Exit(GameManager* owner)
	{
	}
}