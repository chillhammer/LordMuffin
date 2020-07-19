#include "SkelPCH.h"
#include <json.h>
#include <GameObject/GameObjectTemplate.h>
#include <Components/ComponentTemplate.h>
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
		if (m_CreatedFromFile)
		{
			LOG_WARN("Trying to create a created scene: {0}", path);
			return;
		}
		using json = nlohmann::json;

		std::ifstream i(path);
		json j;
		i >> j;

		m_Name = j.at("Name").get<std::string>();

		auto& objects = j["Objects"].items();

		for (auto& entry : objects)
		{
			auto obj = entry.value();
			if (obj.find("ExistsOn") != obj.end())
			{
#ifdef SERVER
				if (obj["ExistsOn"] != "Server")
#else
				if (obj["ExistsOn"] != "Client")
#endif
					continue;
			}
			GameObjectTemplatePtr objectTempl(AddObjectTemplate());

			
			objectTempl->LoadFromJson(obj);
		}

		m_CreatedFromFile = true;
	}
	const std::vector<GameObjectTemplatePtr>& Scene::GetObjectTemplates() const
	{
		return m_ObjectTemplates;
	}
	GameObjectTemplatePtr Scene::AddObjectTemplate()
	{
		GameObjectTemplate* objTempl = new GameObjectTemplate();
		GameObjectTemplatePtr newObjectPtr(objTempl);
		if (newObjectPtr)
		{
			m_ObjectTemplates.emplace_back(newObjectPtr);
		}
		return newObjectPtr;
	}
}
