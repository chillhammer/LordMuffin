#include <SkelPCH.h>
#include <Camera/CameraComponent.h>
#include <Client/ClientManager.h>
#include "Objects/Network/NetworkComponent.h"
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
	// Only use on client. Test if this player is local client
	bool Skel::PlayerComponent::IsLocalClient() const
	{
		if (m_Network->GetPlayerObject(Client.GetClientID()) == m_Owner)
		{
			return true;
		}
		return false;
	}
	void Skel::PlayerComponent::OnCreated()
	{
		ASSERT(Objects::ComponentExists<NetworkComponent>(), "Player component cannot exist without network");
		m_Network = &Objects::FindFirstComponent<NetworkComponent>();
	}
	void Skel::PlayerComponent::Draw()
	{
		if (IsLocalClient())
		{
			CameraComponent& camera = Objects::FindFirstComponent<CameraComponent>();
			camera.SetPivotPosition(m_Owner->ObjectTransform.Position);
		}
	}
}
