#pragma once
#include <Components/ComponentTemplate.h>
#include <json.h>

namespace Skel
{
	// Data only for a game object
	class GameObjectTemplate
	{
	public:
		GameObjectTemplate();
		GameObjectTemplate(const std::string& path); // Prefab file

		std::string Name;
		Vector3 Position;
		Vector3 Rotation;
		Vector3 Scale;

		void LoadFromJson(nlohmann::json& obj);
		ComponentTemplatePtr AddComponentTemplate(const std::string& component);
		class GameObject* Instantiate();

		// TODO: Make children or parent "pointer"/"link". Currently no way to create a child object (Object Transform)
	private:
		std::vector<ComponentTemplatePtr> m_ComponentTemplates;
	};
	typedef std::shared_ptr<GameObjectTemplate> GameObjectTemplatePtr;
	
}