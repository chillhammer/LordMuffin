#include <SkelPCH.h>
#include <Camera/CameraComponent.h>
#include <Client/ClientManager.h>
#include <GameObject/GameObjectManager.h>
#include "Objects/Network/NetworkComponent.h"
#include <Resources/ResourceManager.h>
#include "PlayerSnapshotState.h"
#include <Graphics/Model/ModelRendererComponent.h>
#include <Graphics/Model/ModelAnimationComponent.h>
#include <Input/InputManager.h>
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
	void PlayerComponent::ProcessInput(const PlayerInputState& input, double dt)
	{
		float forward = static_cast<float>((input.Forward ? 1 : 0) - (input.Back ? 1 : 0));
		float side = static_cast<float>((input.Right ? 1 : 0) - (input.Left ? 1 : 0));
		float moveMult = 3.0f;
		float fdt = static_cast<float>(dt);

		m_Owner->ObjectTransform.SetYaw(input.Yaw);
		m_Owner->ObjectTransform.Position += m_Owner->ObjectTransform.GetHeading() * forward * fdt * moveMult;
		m_Owner->ObjectTransform.Position += m_Owner->ObjectTransform.GetSide() * side * fdt * moveMult;
		m_Owner->ObjectTransform.Position += m_Owner->ObjectTransform.GetSide() * (input.Jump ? 1.f : 0.f);

		if (forward > 0)
		{
			m_Animation->PlayOverlayAnimation(PlayerAnim::WalkForward);
		}
		else if (forward < 0)
		{
			m_Animation->PlayOverlayAnimation(PlayerAnim::WalkBackward);
		}
		else if (side > 0)
		{
			m_Animation->PlayOverlayAnimation(PlayerAnim::WalkRight);
		}
		else if (side < 0)
		{
			m_Animation->PlayOverlayAnimation(PlayerAnim::WalkLeft);
		}
		else
		{
			m_Animation->PlayOverlayAnimation(PlayerAnim::Stand);
		}
		// TODO: Other feet controls

#ifndef SERVER
		if (IsLocalClient())
		{
			CameraComponent& camera = Objects.FindFirstComponent<CameraComponent>();
			camera.SetPivotPosition(m_Owner->ObjectTransform.Position);
		}
#endif
	}
	void PlayerComponent::ApplySnapshotState(const PlayerSnapshotState& state)
	{
		m_Owner->ObjectTransform.Position = state.Position;
		m_Owner->ObjectTransform.SetYaw(state.Yaw);
		// TODO: pitch

		m_Animation->PlayAnimation(state.AnimationIndex);
		m_Animation->PlayOverlayAnimation(state.OverlayAnimationIndex);
	}
	// Only use on client. Test if this player is local client
	bool Skel::PlayerComponent::IsLocalClient() const
	{
		if (m_Network->GetLocalPlayerObject() == m_Owner)
		{
			return true;
		}
		return false;
	}
	const uint8 Skel::PlayerComponent::GetCurrentAnimationIndex() const
	{
		return m_Animation->GetCurrentAnimationIndex();
	}
	const uint8 Skel::PlayerComponent::GetOverlayAnimationIndex() const
	{
		return m_Animation->GetOverlayAnimationIndex();
	}
	void Skel::PlayerComponent::OnCreated()
	{
		ASSERT(Objects.ComponentExists<NetworkComponent>(), "Player component cannot exist without network");
		m_Network = &Objects.FindFirstComponent<NetworkComponent>();
		m_Animation = &m_Owner->GetComponent<ModelAnimationComponent>();
		m_Shader = Resources.GetShader(m_Owner->GetComponent<ModelRendererComponent>().GetShaderName());
		m_HeadModel = Resources.GetModel("SoldierHead");
		m_HeadShader = Resources.GetShader("Model");
	}
	void Skel::PlayerComponent::Update()
	{
#ifndef SERVER
		if (IsLocalClient())
		{
			CameraComponent& camera = Objects.FindFirstComponent<CameraComponent>();
			camera.SetPivotPosition(m_Owner->ObjectTransform.Position);
		}
#endif

	}
	void Skel::PlayerComponent::Draw()
	{
		Vector3 headPos = Vector3(0.0f, 2.7f, 0.0f);
		
		Matrix4x4 headMat = Matrix4x4(1.0f);
		headMat = glm::translate(headMat, headPos);
		m_HeadModel->Draw(m_HeadShader, headMat * m_Owner->ObjectTransform.GetMatrix());

		// TODO: Handle main animation
		{
			m_Animation->PlayAnimation(0);
		}
	}
}
