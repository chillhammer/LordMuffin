#pragma once
#include <SkelPCH.h>
#include <Graphics/Model/Mesh.h>
#include <Graphics/Model/Model.h>
#include <Graphics/Model/ImportedSkinnedModel.h>
#include <Graphics/OpenGL/Texture.h>
#include <Graphics/Text/Font.h>

namespace Skel
{
	static void LoadTexture(std::unordered_map<std::string, TexturePtr>& textures, std::string name, std::string file);
	static void LoadCubemap(std::unordered_map<std::string, TexturePtr>& textures, std::string name, std::string file);

	// Seperates game-specific loading behavior from ResourceManager
	// Loads in order of Textures, Meshes, Models, Shaders, Fonts
	void LoadResources(
		std::unordered_map<std::string, TexturePtr>& textures,	std::unordered_map<std::string, MeshPtr>&   meshes,
		std::unordered_map<std::string, ModelPtr>&	 models,	std::unordered_map<std::string, ShaderPtr>& shaders, 
		std::unordered_map<std::string, FontPtr>&	 fonts,		std::unordered_map<std::string, ScenePtr>&	scenes)
	{
		// Textures
		LoadTexture(textures, "Wood",			"../Assets/Textures/wood.png");
		//LoadCubemap(textures, "Sky",			"../Assets/Textures/Skybox/Skybox");
		LoadTexture(textures, "TitleScreen",	"../Assets/Textures/TitleScreen.png");
		LoadTexture(textures, "BlueScoreboard",	"../Assets/Textures/BlueScoreboard.png");
		LoadTexture(textures, "RedScoreboard",	"../Assets/Textures/RedScoreboard.png");


		// Meshes
		meshes.emplace("WoodenBox", new BoxMesh(*Resources.GetTexture("Wood")));


		// Models
		models.emplace("Court",			new ImportedModel("../Assets/Models/Court/court.obj"));
		models.emplace("SuitBlue",		new ImportedModel("../Assets/Models/Suit/suit-blue.obj"));
		models.emplace("SuitRed",		new ImportedModel("../Assets/Models/Suit/suit-red.obj"));
		models.emplace("FedoraAgent",	new ImportedModel("../Assets/Models/FedoraAgent/fedora-agent.obj"));
		models.emplace("Fedora",		new ImportedModel("../Assets/Models/Fedora/fedora.obj"));
		models.emplace("Bendy",			new ImportedSkinnedModel("../Assets/Models/Bendy/Bendy.fbx"));
		models.emplace("Soldier",		new ImportedSkinnedModel("../Assets/Models/Soldier/Soldier.fbx"));
		models.emplace("SoldierHead",	new ImportedModel("../Assets/Models/Soldier/SoldierHead.obj"));
		models.emplace("WoodenBox",		new WoodenBoxModel());


		// Shaders
		shaders.emplace("Basic",		new Shader("Shaders/Basic.shader"));
		shaders.emplace("Model",		new Shader("Shaders/Model.shader"));
		shaders.emplace("SkinnedModel",	new Shader("Shaders/SkinnedModel.shader"));
		shaders.emplace("Debug",		new Shader("Shaders/Debug.shader"));
		shaders.emplace("UI",			new Shader("Shaders/UI.shader"));
		shaders.emplace("Text",			new Shader("Shaders/Text.shader"));
		shaders.emplace("Skybox",		new Shader("Shaders/Skybox.shader"));


		// Fonts
		fonts.emplace("Arial",	new Font("../Assets/Fonts/arial.ttf"));
		fonts.emplace("InGame", new Font("../Assets/Fonts/BunnyFunny.ttf"));

		// Scenes
		scenes.emplace("TestLevel", new Scene("../Assets/Scenes/TestLevel.json"));
	}

	// Specialized loading function for textures due to loading sepearte from contructor
	void LoadTexture(std::unordered_map<std::string, TexturePtr>& textures, std::string name, std::string file)
	{
		TexturePtr texture(new Texture(file));
		texture->LoadTexture();
		textures.emplace(name, texture);
	}
	// Specialized loading function for textures due to loading sepearte from contructor
	void LoadCubemap(std::unordered_map<std::string, TexturePtr>& textures, std::string name, std::string file)
	{
		TexturePtr texture(new Cubemap(file));
		texture->LoadTexture();
		textures.emplace(name, texture);
	}
}