#pragma once
#include "../GameManager.h"
#include <Objects/Box/WoodenBox.h>
#include <Graphics/Model/Skybox.h>
#include <UI/Button.h>
#include <Game/Screens/PauseScreen.h>
#include <Net/Net.h>
#include <Net/FakeLagPacketHolder.h>
#include <Packets/PlayerInputPacket.h>
#include <GameObject/GameObjectTemplate.h>

#include <Objects/Player/PlayerObject.h>
#include <Net/Socket.h>
namespace Skel::GameStates
{

	// Simple Test State to Experiment
	STATE_CLASS_SINGLETON(GameManager, Test,	
		Camera m_Camera;
		Vector3 m_Light;
		ShaderPtr m_Shader;
		ShaderPtr m_SkinnedShader;
		ShaderPtr m_DebugShader;
		WoodenBox m_Box;
		PlayerObject m_Player;
		PlayerObject m_PlayerObjectArray[Net::MAX_PLAYERS];
		Net::Socket m_Client;
		Net::FakeLagPacketHolder<Net::PlayerInputPacket> m_LagInputPackets;
		float m_FrameTime;
	);

	STATE_CLASS_SINGLETON(GameManager, TestEntity,
		Camera m_Camera;
		Vector3 m_Light;
		ShaderPtr m_Shader;
		ShaderPtr m_SkinnedShader;
	);

	STATE_CLASS_SINGLETON(GameManager, MainMenu,
		ShaderPtr m_UIShader;
		Button m_Start;
	);
}