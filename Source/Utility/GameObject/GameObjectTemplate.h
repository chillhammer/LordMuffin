#pragma once
#include <GameObject/GameObject.h>
#include <Components/ComponentTemplate.h>

namespace Skel
{
	// Data only for a game object

	class GameObjectTemplate
	{
	public:
		GameObjectTemplate();

		Vector3 Position;
		Vector3 Rotation;
		Vector3 Scale;

		ComponentTemplatePtr AddComponentTemplate(const std::string& component);
		GameObjectPtr Instantiate();

		// TODO: Make children or parent "pointer"/"link"
	private:
		std::vector<ComponentTemplatePtr> m_ComponentTemplates;
	};

	
}