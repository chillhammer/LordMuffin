#include "SkelPCH.h"
#include <json.h>
#include <GameObject/GameObjectTemplate.h>
#include <Components/ComponentTemplate.h>
#include <fstream>
#include "Scene.h"

using nlohmann::json;



namespace Skel {

	void to_json(json& j, const Vector3& v) {
		j = json{ {"x", v.x}, {"y", v.y}, {"z", v.z} };
	}

	void from_json(const json& j, Vector3& v) {
		j[0].get_to(v.x);
		j[1].get_to(v.y);
		j[2].get_to(v.z);
	}

	#define JSON_TO_VARIANT(t) if(type.get_id() == type.get<t>().get_id()){return val.get<t>();}
	// Used for dynamic component generation
	rttr::variant ConvertJsonTypeToVariant(json::value_type val, rttr::type type)
	{
		JSON_TO_VARIANT(int);
		JSON_TO_VARIANT(float);
		JSON_TO_VARIANT(std::string);
		JSON_TO_VARIANT(std::vector<std::string>);

		ASSERT(false, "Failed to convert Json Type");
		return rttr::variant();
	}
}

namespace nlohmann {
	template<>
	struct adl_serializer<Vector3> {
		static void to_json(json& j, const Vector3& value) {
			// calls the "to_json" method in T's namespace
			Skel::to_json(j, value);
		}

		static void from_json(const json& j, Vector3& value) {
			// same thing, but with the "from_json" method
			Skel::from_json(j, value);
		}
	};
}



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
			GameObjectTemplatePtr objectTempl(AddObjectTemplate());

			auto obj = entry.value();
			// Object optional properties
			if (obj.find("Name") != obj.end())
			{
				objectTempl->Name = obj.at("Name").get<std::string>();
			}
			if (obj.find("Position") != obj.end())
			{
				objectTempl->Position = obj.at("Position").get<Vector3>();
			}
			if (obj.find("Rotation") != obj.end())
			{
				objectTempl->Rotation = obj.at("Rotation").get<Vector3>();
			}
			if (obj.find("Scale") != obj.end())
			{
				objectTempl->Scale = obj.at("Scale").get<Vector3>();
			}

			std::cout << "Obj: " << obj << std::endl;

			if (obj.find("Components") != obj.end())
			{
				for (auto& componentEntry : obj.at("Components").items())
				{
					std::string typeName = componentEntry.key();
					rttr::type componentType = rttr::type::get_by_name(typeName);
					ASSERT(componentType.is_valid(), "Component Type is not valid");
					ComponentTemplatePtr componentTempl(objectTempl->AddComponentTemplate(typeName));
					

					auto& componentProps = componentEntry.value();
					for (auto& prop : componentProps.items())
					{
						std::string propName = prop.key();
						rttr::type propType = componentType.get_property(propName).get_type();
						rttr::variant propVal = ConvertJsonTypeToVariant(prop.value(), propType);
						
						componentTempl->SetProperty(propName, propVal);
					}
					//ComponentPtr component = componentTempl->Instatiate(); // temp
					
				}
			}
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
