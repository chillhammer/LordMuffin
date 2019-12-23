#include <skelpch.h>
#include <Input/InputManager.h>
#include <Resources/ResourceManager.h>
#include <Packets/PlayerInputPacket.h>
#include <imgui.h>
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

		m_DebugShader = Resources.GetShader("Debug");

		m_Box.ObjectTransform.Scale = Vector3(15, 0.001f, 30);

		m_Player.ObjectTransform.Position = Vector3(2.0f, 1.0f, 0.0f);
	}
	void Test::Execute(GameManager* owner)
	{
		m_Camera.Update();

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_ViewProjection", m_Camera.GetProjectionMatrix() * m_Camera.GetViewMatrix());
		m_DebugShader->Bind();
		m_DebugShader->SetUniformMat4f("u_ViewProjection", m_Camera.GetProjectionMatrix() * m_Camera.GetViewMatrix());

		ImGui::Begin("Demo window");
		ImGui::Button("Hello!");
		ImGui::End();


		// Client Send
		{
			Net::Buffer buffer;
			
			PlayerInputState input = { Input.IsKeyDown(KEY_W), Input.IsKeyDown(KEY_S) };
			PlayerInputPacket packet(input);
			packet.WriteToBuffer(buffer);
			Net::Address serverAddress = Net::GetServerAddress();
			m_Client.SendBuffer(buffer, serverAddress); // don't use m_Client
		}

		// Client Receive
		Net::Buffer receiveBuffer;
		Net::Address serverAddress;
		PlayerInputState input;
		while (m_Client.ReceiveBuffer(receiveBuffer, serverAddress)) {

			receiveBuffer.ResetReadPosition();
			PlayerInputPacket packet;
			packet.ReadFromBuffer(receiveBuffer);
			input = packet.InputState;

			m_Player.ProcessInput(input, Game.DeltaTime());
		}
		
		

		m_Box.Draw();

		m_Player.Draw();

	}
	void Test::Exit(GameManager* owner)
	{
	}
}