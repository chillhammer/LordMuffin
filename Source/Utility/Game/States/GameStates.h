#pragma once
#include "../GameManager.h"
#include <Objects/Box/WoodenBox.h>
#include <Graphics/Model/Skybox.h>
#include <UI/Button.h>
#include <Game/Screens/PauseScreen.h>
#include <Net/Net.h>

#include <Objects/Player/PlayerObject.h>
#include <Net/Socket.h>
namespace Skel::GameStates
{

	// Simple Test State to Experiment
	STATE_CLASS_SINGLETON(GameManager, Test,	
		Camera m_Camera;
		Vector3 m_Light;
		ShaderPtr m_Shader;
		ShaderPtr m_DebugShader;
		WoodenBox m_Box;
		PlayerObject m_Player;
		PlayerObject m_PlayerObjectArray[Net::MAX_PLAYERS];
		Net::Socket m_Client;
	);

	STATE_CLASS_SINGLETON(GameManager, MainMenu,
		ShaderPtr m_UIShader;
		Button m_Start;
	);
}