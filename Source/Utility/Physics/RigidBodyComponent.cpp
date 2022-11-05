#include "SkelPCH.h"
#include <rttr/registration>
#include <Game/GameManager.h>
#include "ColliderComponent.h"
#include "PhysicsWorldManager.h"
#include "RigidBodyComponent.h"

namespace Skel
{
	RTTR_REGISTRATION
	{
		using namespace rttr;
		registration::class_<RigidBodyComponent>("RigidBody")
			.constructor<>(registration::public_access);
	}

	RigidBodyComponent::RigidBodyComponent() : m_Velocity ( 0, 0, 0)
	{
	}

	void Skel::RigidBodyComponent::OnCreated()
	{
		m_Collider = &m_Owner->GetComponent<ColliderComponent>();
	}

	void RigidBodyComponent::OnSceneCreated()
	{
	}

	void RigidBodyComponent::Update()
	{
		float dt = Game.DeltaTime();
		UpdatePosition( dt );
	}

	// Simulates rigidbody movement
	// On Client - updates it in game loop normally
	// On Server - updates it from Input packet using Client delta time. May want to refactor it on Server
	void RigidBodyComponent::UpdatePosition( double dt )
	{
		GameObject* owner = GetOwner();
		owner->ObjectTransform.Position += m_Velocity * static_cast<float>(dt);

		// TODO: Collider check
		std::vector<class RigidBodyComponent*>& staticRigidBodies = PhysicsWorld.GetStaticRigidBodies();
		for(RigidBodyComponent* otherRigidBody : staticRigidBodies)
		{
			ASSERT(otherRigidBody->GetCollider(), "Static rigid body must have a collider");
			Vector3 resolveVec;
			if (GetCollider() != otherRigidBody->GetCollider() && GetCollider()->IsColliding(*otherRigidBody->GetCollider(), resolveVec))
			{
				LOG("Two objects are colliding! {0} and {1}", owner->GetName(), otherRigidBody->GetOwner()->GetName());
				owner->ObjectTransform.Position -= resolveVec;
				m_Velocity -= resolveVec;
			}
		}
	}

	void Skel::RigidBodyComponent::SetVelocity(Vector3 velocity)
	{
		m_Velocity = velocity;
	}
	void Skel::RigidBodyComponent::AddVelocity(Vector3 velocity)
	{
		m_Velocity += velocity;
	}
	void Skel::RigidBodyComponent::ClampVelocity(float maxSpeed)
	{
		float speed = glm::length(m_Velocity);
		if (speed <= 0.0f)
		{
			return;
		}
		speed = glm::clamp(speed, 0.0f, maxSpeed);
		m_Velocity = glm::normalize(m_Velocity) * speed;
	}
	Vector3 Skel::RigidBodyComponent::GetVelocity() const
	{
		return m_Velocity;
	}
	ColliderComponent* Skel::RigidBodyComponent::GetCollider()
	{
		return m_Collider;
	}
	float Skel::RigidBodyComponent::GetSpeed() const
	{
		return glm::length(m_Velocity);
	}
	void Skel::RigidBodyComponent::ReduceVelocity(float reduceSpeed)
	{
		float speed = glm::length(m_Velocity);
		if (speed <= 0.0f)
		{
			return;
		}
		speed = glm::clamp(speed - reduceSpeed, 0.0f, speed);
		m_Velocity = glm::normalize(m_Velocity) * speed;
	}
}
