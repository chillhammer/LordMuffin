#pragma once
#include <Game/GameManager.h>
#include <StateMachine/State.h>
#include <StateMachine/StateMachine.h>
#include <GameObject/GameObjectTemplate.h>

namespace Skel
{
	// Collection of entities that encompasses what is shown on screen
	class Scene
	{
	public:
		Scene() {};
		Scene(const std::string&);
		void CreateFromFile(const std::string& path);
		const std::vector<GameObjectTemplatePtr>& GetObjectTemplates() const;
	private:
		GameObjectTemplatePtr AddObjectTemplate();

		std::vector<GameObjectTemplatePtr> m_ObjectTemplates;
		bool m_CreatedFromFile = false;
		std::string m_Name;
	};
	typedef std::shared_ptr<Scene> ScenePtr;
}