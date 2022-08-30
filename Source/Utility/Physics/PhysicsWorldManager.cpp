#include "SkelPCH.h"
#include "GameObject/GameObjectManager.h"
#include "RigidBodyComponent.h"
#include "ColliderComponent.h"
#include "EventSystem/Events/GameObjectEvent.h"
#include "PhysicsWorldManager.h"

namespace Skel
{
    PhysicsWorldManager& Skel::PhysicsWorldManager::Instance()
    {
        static PhysicsWorldManager instance;
        return instance;
    }

    void PhysicsWorldManager::Init()
    {
        Objects.SceneLoaded.AddObserver(this);
    }

    void PhysicsWorldManager::OnEvent(const Subject* subject, Event& event)
    {
        Evnt::Dispatch<SceneLoadedEvent>(event, EVENT_BIND_FN(PhysicsWorldManager, OnSceneLoaded));
    }

    PhysicsWorldManager::~PhysicsWorldManager()
    {
    }

    bool PhysicsWorldManager::OnSceneLoaded(SceneLoadedEvent& e)
    {
        for (GameObject* gameObject : e.GameObjects)
        {
            if (gameObject->HasComponent<RigidBodyComponent>())
            {
                RigidBodyComponent& rigidBody = gameObject->GetComponent<RigidBodyComponent>();
                // TODO: Filter out rigid bodies that aren't static
                if( rigidBody.GetCollider() != nullptr )
                {
                    m_StaticRigidBodies.push_back(&rigidBody);
                }
            }
        }
        return false;
    }

    std::vector<class RigidBodyComponent*>& PhysicsWorldManager::GetStaticRigidBodies()
    {
        return m_StaticRigidBodies;
    }

}
