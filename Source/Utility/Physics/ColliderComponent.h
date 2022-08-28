#pragma once
#include <Components/GameObjectComponent.h>
#include <glm/vec3.hpp>

namespace Skel
{
	struct SphereCollider
	{
		float radius;
	};
	struct AABBCollider
	{
		Vector3 center;
		Vector3 halfExtents;
	};
	enum ColliderType
	{
		COLLIDER_SPHERE,
		COLLIDER_AABB,

		COLLIDER_TYPE_SIZE
	};
	extern std::string ColliderTypeNames[];
	
	union ColliderData
	{
		SphereCollider Sphere;
		AABBCollider AABB;
	};

	typedef glm::vec3 Vector3;
	class GameObjectComponent;
	class ColliderComponent : public GameObjectComponent
	{
	public:
		ColliderComponent();

		//virtual void OnSceneCreated() override;
		//virtual void Update() override;

		void SetColliderType(const std::string& type);
		const std::string& GetColliderType() const;

		void SetRadius(const std::string& radius);
		const std::string& GetRadius() const;

		void SetCenter(const std::string& center);
		const std::string& GetCenter() const;

		void SetHalfExtents(const std::string& halfExtents);
		const std::string& GetHalfExtents() const;

		// TODO: Add collider AABB getters and setters

		bool IsColliding( const GameObject* otherObj ) const;
		bool IsColliding( const GameObject* otherObj, Vector3& outResolveVec ) const;
		bool IsColliding( const ColliderComponent& otherCollider ) const;
		bool IsColliding( const ColliderComponent& otherCollider, Vector3& outResolveVec ) const;
		friend class ColliderComponent;
		RTTR_ENABLE(GameObjectComponent)
		RTTR_REGISTRATION_FRIEND
	private:
		ColliderType m_ColliderType;
		ColliderData m_ColliderData;
	};
}