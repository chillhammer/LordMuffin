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
		m_Camera.Init();
		m_Light = Vector3(0.f, 10.f, -5.f);

		m_Shader = Resources.GetShader("Model");
		m_Shader->Bind();
		m_Shader->SetUniform3f("u_LightPosition", m_Light.x, m_Light.y, m_Light.z);

		m_SkinnedShader = Resources.GetShader("SkinnedModel");
		m_SkinnedShader->Bind();
		m_SkinnedShader->SetUniform3f("u_LightPosition", m_Light.x, m_Light.y, m_Light.z);

		auto modelRend = m_ObjectTemplate.AddComponentTemplate("ModelRenderer");
		modelRend->SetProperty("Model", "WoodenBox");

		m_ObjectTemplate.Scale = Vector3(15, 0.001f, 30);
		m_ObjectPtr = m_ObjectTemplate.Instantiate();

		m_ObjectTemplate2.AddComponentTemplate("ModelRenderer")->SetProperty("Model", "Bendy");
		m_ObjectTemplate2.AddComponentTemplate("ModelAnimation");

		m_ObjectTemplate2.Position.z = 5.0f;
		m_ObjectPtr2 = m_ObjectTemplate2.Instantiate();
	}
	void TestEntity::Execute(GameManager* owner)
	{
		m_Camera.Update();

		

		m_ObjectPtr->DrawComponents();

		m_ObjectPtr2->DrawComponents();

		///////////////////////////
		// Update Camera
		// m_Camera.SetPivotPosition(m_PlayerObjectArray[Client.GetClientID()].ObjectTransform.Position);

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_ViewProjection", m_Camera.GetProjectionMatrix() * m_Camera.GetViewMatrix());
		m_SkinnedShader->Bind();
		m_SkinnedShader->SetUniformMat4f("u_ViewProjection", m_Camera.GetProjectionMatrix() * m_Camera.GetViewMatrix());
	}
	void TestEntity::Exit(GameManager* owner)
	{
	}
}