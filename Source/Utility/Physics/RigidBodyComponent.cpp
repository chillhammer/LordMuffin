#include "SkelPCH.h"
#include <rttr/registration>
#include <Game/GameManager.h>
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

	void RigidBodyComponent::OnSceneCreated()
	{
	}

	void RigidBodyComponent::Update()
	{
		float dt = Game.DeltaTime();
		UpdatePosition( dt );
	}

	void RigidBodyComponent::UpdatePosition( double dt )
	{
		GameObject* owner = GetOwner();
		owner->ObjectTransform.Position += m_Velocity * static_cast<float>(dt);
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
