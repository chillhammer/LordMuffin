#include <skelpch.h>
#include <Window/Window.h>
#include <Input/InputManager.h>
#include <Game/GameManager.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <Graphics/OpenGL/Renderer.h>
#include <Graphics/OpenGL/IndexBuffer.h>
#include <Graphics/OpenGL/Shader.h>
#include <Graphics/OpenGL/VertexBuffer.h>
#include <Graphics/OpenGL/VertexArray.h>
#include <Graphics/OpenGL/Texture.h>
#include <Objects/Box/BoxMesh.h>
#include <glm/gtc/matrix_transform.hpp>
#include <Graphics/Model/Model.h>
#include <Graphics/Model/ImportedModel.h>
#include <Graphics/Model/SingleMeshModel.h>
#include <Resources/ResourceManager.h>
#include <Objects/Box/WoodenBox.h>
#include <Game/States/GameStates.h>
#include <Net/Net.h>
#include <Net/Socket.h>
#include <Server/ServerManager.h>

using namespace Skel;

namespace Skel {
	WindowProps g_WindowProperties("Ragged Skeleton", 1280, 780);
}

int main()
{
	Log::Init();
#ifdef SERVER
	Server.ServerMain();
	return 0;
#endif

	// Setting Up Dependencies
	Input.Init();
	Game.Init();
	Net::Init();
	// Loads All Resources
	Resources.Init();

	Renderer renderer;
	Game.Start();
	Game.ChangeState(Skel::GameStates::Test::Instance());

	while (Game.IsRunning())
	{
		// App Render
		renderer.Clear();

		Game.Run();
	}



	return 0;
}