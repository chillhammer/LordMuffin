#pragma once
#include <glm/vec3.hpp>

namespace Skel
{
	typedef glm::vec3 Vector3;
	class GameObjectComponent;
	class RigidBodyComponent : public GameObjectComponent
	{
	public:
		RigidBodyComponent();

		virtual void OnSceneCreated() override;
		virtual void Update() override;

		void SetVelocity( Vector3 velocity );
		void AddVelocity( Vector3 velocity );
		void ClampVelocity( float);
		void ReduceVelocity(float reduceSpeed);
		float GetSpeed() const;
		Vector3 GetVelocity() const;
		void UpdatePosition(double dt);

		RTTR_ENABLE(GameObjectComponent)
		RTTR_REGISTRATION_FRIEND
	private:
		Vector3 m_Velocity;
	};
}