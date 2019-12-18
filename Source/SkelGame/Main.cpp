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

using namespace Skel;

namespace Skel {
	WindowProps g_WindowProperties("Ragged Skeleton", 1280, 780);
}

int main()
{
	// Setting Up Dependencies
	Log::Init();
	Input.Init();
	Game.Init();
	Net::Init();
	// Loads All Resources
	Resources.Init();

	Renderer renderer;
	Game.Start();
	Game.ChangeState(Skel::GameStates::Test::Instance());

#ifdef SERVER
	Net::Socket server;
	server.Bind(Net::GetServerAddress().GetPort());
#endif
	while (Game.IsRunning())
	{
		// App Render
		renderer.Clear();

#ifndef SERVER
		Game.Run();
#else
		Net::Buffer buffer;
		Net::Address fromAddress;
		// Send back
		if (server.ReceiveBuffer(buffer, fromAddress)) {
			server.SendBuffer(buffer, fromAddress);

			PlayerInputState input;
			memcpy(&input, buffer.Data(), buffer.Length());
			LOG("Got buffer, state= ({0},{1})", input.Forward, input.Back);
		}
#endif

	}



	return 0;
}