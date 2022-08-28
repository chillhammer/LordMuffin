#include "SkelPCH.h"
#include <rttr/registration>
#include <Game/GameManager.h>
#include "ColliderComponent.h"

namespace Skel
{
	RTTR_REGISTRATION
	{
		using namespace rttr;
		registration::class_<ColliderComponent>("Collider")
			.constructor<>(registration::public_access)
			.property("Type", &ColliderComponent::GetColliderType, &ColliderComponent::SetColliderType)
			.property("Radius", &ColliderComponent::GetRadius, &ColliderComponent::SetRadius);
	}

	std::string ColliderTypeNames[] = {
		"Sphere",
		"AABB"
	};

	ColliderComponent::ColliderComponent()
	{
	}
	void Skel::ColliderComponent::SetColliderType(const std::string& type)
	{
		for (int colliderIndex = 0; colliderIndex < COLLIDER_TYPE_SIZE; colliderIndex++)
		{
			if (ColliderTypeNames[colliderIndex] == type)
			{
				m_ColliderType = static_cast<ColliderType>(colliderIndex);
			}
		}
	}
	const std::string& Skel::ColliderComponent::GetColliderType() const
	{
		return ColliderTypeNames[static_cast<int>(m_ColliderType)];
	}
	void Skel::ColliderComponent::SetRadius(const std::string& radius)
	{
		m_ColliderData.Sphere.radius = std::stof(radius);
	}
	const std::string& Skel::ColliderComponent::GetRadius() const
	{
		return std::to_string(m_ColliderData.Sphere.radius);
	}
	bool Skel::ColliderComponent::IsColliding(const GameObject* otherObj) const
	{
		Vector3 resolveVec;
		return IsColliding( otherObj, resolveVec );
	}
	bool Skel::ColliderComponent::IsColliding(const GameObject* otherObj, Vector3& outResolveVec) const
	{
		if (otherObj == nullptr)
		{
			return false;
		}
		if (!otherObj->HasComponent<ColliderComponent>())
		{
			return false;
		}
		ColliderComponent& otherCollider = otherObj->GetComponent<ColliderComponent>();
		if (IsColliding(otherCollider, outResolveVec))
		{
			return true;
		}
		return false;
	}
	bool Skel::ColliderComponent::IsColliding(const ColliderComponent& otherCollider) const
	{
		Vector3 resolveVec;
		return IsColliding( otherCollider, resolveVec );
	}

	bool Skel::ColliderComponent::IsColliding(const ColliderComponent& otherCollider, Vector3& outResolveVec ) const
	{
		Vector3 origin = GetOwner()->ObjectTransform.Position;
		Vector3 otherOrigin = otherCollider.GetOwner()->ObjectTransform.Position;
		if (m_ColliderType == COLLIDER_SPHERE && otherCollider.m_ColliderType == COLLIDER_SPHERE)
		{
			float radius = m_ColliderData.Sphere.radius;
			float otherRadius = otherCollider.m_ColliderData.Sphere.radius;
			float originDistanceSq = glm::distance2(otherOrigin, origin);
			float radiusSum = (otherRadius + radius);
			if (originDistanceSq < radiusSum * radiusSum)
			{
				ASSERT( originDistanceSq > 0, "Distance must be bigger than 0, otherwise resolve cannot happen");
				float originDistance = glm::sqrt(originDistanceSq);
				Vector3 otherToMe = ( origin - otherOrigin ) / originDistance;
				outResolveVec = otherToMe * (radiusSum - originDistance );
				return true;
			}
		}

		return false;
	}
}
