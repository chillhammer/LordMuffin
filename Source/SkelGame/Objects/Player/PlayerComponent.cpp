#include <SkelPCH.h>
#include "PlayerSnapshotState.h"
#include "PlayerComponent.h"

namespace Skel
{
	RTTR_REGISTRATION
	{
		using namespace rttr;
		registration::class_<PlayerComponent>("Player")
			.constructor<>(registration::public_access);
	}

	PlayerComponent::PlayerComponent()
	{
	}
	void PlayerComponent::ProcessInput(const PlayerInputState& input, float dt)
	{
		float forward = (input.Forward ? 1 : 0) - (input.Back ? 1 : 0);
		float side = (input.Right ? 1 : 0) - (input.Left ? 1 : 0);
		float moveMult = 3.0f;

		m_Owner->ObjectTransform.SetYaw(input.Yaw);
		m_Owner->ObjectTransform.Position += m_Owner->ObjectTransform.GetHeading() * forward * dt * moveMult;
		m_Owner->ObjectTransform.Position += m_Owner->ObjectTransform.GetSide() * side * dt * moveMult;
		m_Owner->ObjectTransform.Position += m_Owner->ObjectTransform.GetSide() * (input.Jump ? 1.f : 0.f);

	}
	void PlayerComponent::ApplySnapshotState(const PlayerSnapshotState& state)
	{
		m_Owner->ObjectTransform.Position = state.Position;
		m_Owner->ObjectTransform.SetYaw(state.Yaw);
		// TODO: pitch

		//m_AnimationController.PlayAnimation(state.AnimationIndex);
	}
}
