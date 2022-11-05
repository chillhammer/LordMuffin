#include "SkelPCH.h"
#include <rttr/registration>
#include <Game/GameManager.h>
#include <Resources/ResourceManager.h>
#include "Macros.h"
#include <imgui.h>
#include "ColliderComponent.h"

namespace Skel
{
	RTTR_REGISTRATION
	{
		using namespace rttr;
		registration::class_<ColliderComponent>("Collider")
			.constructor<>(registration::public_access)
			.property("Type", &ColliderComponent::GetColliderType, &ColliderComponent::SetColliderType)
			.property("Radius", &ColliderComponent::GetRadius, &ColliderComponent::SetRadius)
			.property("Center", &ColliderComponent::GetCenter, &ColliderComponent::SetCenter)
			.property("HalfExtents", &ColliderComponent::GetHalfExtents, &ColliderComponent::SetHalfExtents);
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

	void Skel::ColliderComponent::SetCenter(const std::string& center)
	{
		Vector3 vec;
		STRING_TO_VECTOR3( center, vec )

		m_Center = vec;
		
	}
	const std::string& Skel::ColliderComponent::GetCenter() const
	{
		std::string str;
		Vector3 vec;

		VECTOR3_TO_STRING(m_Center, str)
		return str;
	}

	void Skel::ColliderComponent::SetHalfExtents(const std::string& halfExtents)
	{
		Vector3 vec;
		STRING_TO_VECTOR3(halfExtents, vec)

		m_ColliderData.AABB.halfExtents = vec;
	}
	const std::string& Skel::ColliderComponent::GetHalfExtents() const
	{
		std::string str;
		VECTOR3_TO_STRING(m_ColliderData.AABB.halfExtents, str)
		return str;
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
		
		
		if (m_ColliderType == COLLIDER_SPHERE && otherCollider.m_ColliderType == COLLIDER_SPHERE)
		{
			Vector3 origin = GetOwner()->ObjectTransform.Position + m_Center;
			Vector3 otherOrigin = otherCollider.GetOwner()->ObjectTransform.Position + otherCollider.m_Center;
			float radius = otherCollider.m_ColliderData.Sphere.radius;
			float otherRadius = otherCollider.m_ColliderData.Sphere.radius;
			float originDistanceSq = glm::distance2(otherOrigin, origin);
			float radiusSum = (otherRadius + radius);
			if (originDistanceSq < radiusSum * radiusSum)
			{
				if( originDistanceSq > 0)
				{
					float originDistance = glm::sqrt(originDistanceSq);
					Vector3 otherToMe = ( origin - otherOrigin ) / originDistance;
					outResolveVec = otherToMe * (radiusSum - originDistance );
				}
				else
				{
					outResolveVec = Vector3( 0, 0, 0);
				}
				return true;
			}
		}
		else if (m_ColliderType == COLLIDER_SPHERE && otherCollider.m_ColliderType == COLLIDER_AABB)
		{
			// TODO: fill out AABB / Sphere collision properly
			Vector3 origin = GetOwner()->ObjectTransform.Position + m_Center;
			Vector3 otherOrigin = otherCollider.GetOwner()->ObjectTransform.Position;

			float radius = m_ColliderData.Sphere.radius;

			float halfExtents = 0.0f;
			float AABBAxisPos = 0.0f;
			float axisDist = 0.0f;
			float sphereAxisPos = 0.0f;
			float smallestIntersectionDist = -1.0f;
			float AABBClosestPosToSphere = -1.0f;
			float sphereClosestPosToAABB = -1.0f;
			int pushDir = 0;
			Vector3 axis;
			Vector3 resolveVec = Vector3( 0, 0, 0);
			// LOG( "-----");

			for( int i = 0; i < 3; i++)
			{
				axis = Vector3( 0, 0, 0 );
				axis[i] = 1;
				halfExtents = otherCollider.m_ColliderData.AABB.halfExtents[i];
				AABBAxisPos = otherCollider.m_Center[i] + otherOrigin[i];
				sphereAxisPos = origin[i];

				pushDir = glm::sign(AABBAxisPos - sphereAxisPos);
				axisDist = glm::abs( AABBAxisPos - sphereAxisPos );
				AABBClosestPosToSphere = AABBAxisPos + glm::min(axisDist, halfExtents) * pushDir * -1;
				sphereClosestPosToAABB = sphereAxisPos + glm::min(axisDist, radius ) * pushDir;

				// float intersectionDist = /* halfExtents + */ radius - axisDist;
				float intersectionDist = ( AABBClosestPosToSphere - sphereClosestPosToAABB ) * pushDir * -1;
				// LOG( "Axis {0} -- Intersection Dist: {1}", i, intersectionDist );
				if (intersectionDist > 0 && (smallestIntersectionDist < 0.0f || intersectionDist < smallestIntersectionDist) )
				{
					smallestIntersectionDist = intersectionDist;
				
					resolveVec = axis * static_cast<float>(pushDir) * intersectionDist;
				}
				if (intersectionDist <= 0)
				{
					return false;
				}
			}

			if (smallestIntersectionDist > 0.0f)
			{
				outResolveVec = resolveVec;
				return true;
			}
		}

		return false;
	}

	bool ColliderDebugSpheres = false;
	bool ColliderDebugABB = false;
	void Skel::ColliderComponent::Draw()
	{
		
		if( m_ColliderType == COLLIDER_AABB )
		{
			if(ColliderDebugABB)
			{
				DEBUG_BOX( ( GetOwner()->ObjectTransform.GetGlobalPosition() + m_Center ), m_ColliderData.AABB.halfExtents )
			}
		}
		else if (m_ColliderType == COLLIDER_SPHERE)
		{
			if(ColliderDebugSpheres)
			{
				DEBUG_SPHERE(GetOwner()->ObjectTransform.GetGlobalPosition() + m_Center, Vector3( m_ColliderData.Sphere.radius ) )
			}
		}
	}
	
	void Skel::ColliderComponent::StaticUpdate()
	{
	#ifdef CLIENT
		ImGui::Begin("Collider Debug");
		ImGui::Checkbox("Collider - Debug Spheres", &ColliderDebugSpheres);
		ImGui::Checkbox("Collider - Debug ABB", &ColliderDebugABB);
		ImGui::End();
	#endif
	}
}
