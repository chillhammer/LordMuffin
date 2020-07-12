#include "SkelPCH.h"
#include <json.h>
#include <fstream>
#include "GameObject.h"
#include "GameObjectTemplate.h"

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
	GameObjectTemplate::GameObjectTemplate() : Position(0.0f), Rotation(0.0f), Scale(1.0f)
	{
	}
	// This constructor allows game objects to be created from a file, aka prefab files
	GameObjectTemplate::GameObjectTemplate(const std::string& path)
	{
		std::ifstream i(path);
		json j;
		i >> j;
		LoadFromJson(j);
	}
	// Creates a game object template from a json object
	// This can be used for scene loading or prefab loading
	void GameObjectTemplate::LoadFromJson(nlohmann::json& obj)
	{
		// Object optional properties
		if (obj.find("Name") != obj.end())
		{
			this->Name = obj.at("Name").get<std::string>();
		}
		if (obj.find("Position") != obj.end())
		{
			this->Position = obj.at("Position").get<Vector3>();
		}
		if (obj.find("Rotation") != obj.end())
		{
			this->Rotation = obj.at("Rotation").get<Vector3>();
		}
		if (obj.find("Scale") != obj.end())
		{
			this->Scale = obj.at("Scale").get<Vector3>();
		}
		// LOG("Loaded Object: {0}", obj);

		if (obj.find("Components") != obj.end())
		{
			for (auto& componentEntry : obj.at("Components").items())
			{
				std::string typeName = componentEntry.key();
				rttr::type componentType = rttr::type::get_by_name(typeName);
				ASSERT(componentType.is_valid(), "Component Type is not valid");
				ComponentTemplatePtr componentTempl(this->AddComponentTemplate(typeName));


				auto& componentProps = componentEntry.value();
				for (auto& prop : componentProps.items())
				{
					std::string propName = prop.key();
					rttr::type propType = componentType.get_property(propName).get_type();
					rttr::variant propVal = ConvertJsonTypeToVariant(prop.value(), propType);

					componentTempl->SetProperty(propName, propVal);
				}
			}
		}
	}
	ComponentTemplatePtr GameObjectTemplate::AddComponentTemplate(const std::string& component)
	{
		rttr::type componentType = rttr::type::get_by_name(component);
		ComponentTemplate* compTemp = new ComponentTemplate(componentType);
		ComponentTemplatePtr newComponentPtr(compTemp);
		if (newComponentPtr)
		{
			m_ComponentTemplates.emplace_back(newComponentPtr);
		}
		return newComponentPtr;
	}

	// Create game object from game object template
	GameObject* GameObjectTemplate::Instantiate()
	{
		GameObject* obj = new GameObject();
		obj->ObjectTransform.Position = Position;
		obj->ObjectTransform.Rotation = Rotation;
		obj->ObjectTransform.Scale = Scale;

		for (auto& compTemp : m_ComponentTemplates)
		{
			ComponentPtr comp = compTemp->Instatiate();
			obj->AddComponent(comp);
		}

		for (ComponentPtr comp : obj->m_Components)
		{
			comp->OnCreated();
		}
		
		return obj;
	}
}
