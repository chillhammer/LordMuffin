
#include <queue>
#include "EventSystem/Events/GameObjectEvent.h"
#include <EventSystem/IObserver.h>

#define PhysicsWorld PhysicsWorldManager::Instance()
namespace Skel
{
	/**
		Manager singleton object handling the physics world
		Mostly dealing with in-game logic.

	**/
	class PhysicsWorldManager : IObserver
	{
	public:
		static PhysicsWorldManager& Instance();
		void Init();
		void OnEvent(const Subject* subject, Event& event);
		bool OnSceneLoaded(SceneLoadedEvent& e);

		std::vector<class RigidBodyComponent*>& GetStaticRigidBodies();

		~PhysicsWorldManager();
	private:
		PhysicsWorldManager() {};

		// Properties
		std::vector<class RigidBodyComponent*> m_StaticRigidBodies;

	private:

	};
}