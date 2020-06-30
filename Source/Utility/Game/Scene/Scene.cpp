#include "SkelPCH.h"
#include <json.h>
#include <fstream>
#include "Scene.h"

namespace Skel
{
	Scene::Scene(const std::string& path)
	{
		CreateFromFile(path);
	}
	void Scene::CreateFromFile(const std::string& path)
	{
		using json = nlohmann::json;

		std::ifstream i(path);
		//json j;
		//i >> j;

		m_CreatedFromFile = true;
	}
}
