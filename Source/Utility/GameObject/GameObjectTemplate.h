#pragma once
#include <Components/ComponentTemplate.h>

namespace Skel
{
	// Data only for a game object

	class GameObjectTemplate
	{
	public:
		GameObjectTemplate();

		std::string Name;
		Vector3 Position;
		Vector3 Rotation;
		Vector3 Scale;

		ComponentTemplatePtr AddComponentTemplate(const std::string& component);
		class GameObject* Instantiate();

		// TODO: Make children or parent "pointer"/"link"
	private:
		std::vector<ComponentTemplatePtr> m_ComponentTemplates;
	};
	typedef std::shared_ptr<GameObjectTemplate> GameObjectTemplatePtr;
	
}