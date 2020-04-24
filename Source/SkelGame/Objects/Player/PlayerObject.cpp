#include "SkelPCH.h"
#include "PlayerObject.h"
#include <Resources/ResourceManager.h>
#include "PlayerSnapshotState.h"

namespace Skel {
	PlayerObject::PlayerObject() : GameObject("Soldier"), m_HeadModel(Resources.GetModel("SoldierHead")), 
		m_AnimationController(m_Model){}

	void PlayerObject::ProcessInput(const PlayerInputState& input, float dt)
	{
		float forward = (input.Forward ? 1 : 0) - (input.Back ? 1 : 0);
		float side = (input.Right ? 1 : 0) - (input.Left ? 1 : 0);
		float moveMult = 3.0f;

		ObjectTransform.SetYaw(input.Yaw);
		ObjectTransform.Position += ObjectTransform.GetHeading() * forward * dt * moveMult;
		ObjectTransform.Position += ObjectTransform.GetSide() * side * dt * moveMult;
		
		ObjectTransform.Position += ObjectTransform.GetSide() * (input.Jump ? 1.f : 0.f);

	}
	// Process animation via latest input.
	void PlayerObject::ProcessAnimation(const PlayerInputState& input)
	{
		float forward = (input.Forward ? 1 : 0) - (input.Back ? 1 : 0);
		if (forward != 0) {
			m_AnimationController.PlayAnimation(PlayerAnimation::TPose);
		}
		else {
			m_AnimationController.PlayAnimation(PlayerAnimation::Stand);
		}
	}
	void PlayerObject::ApplySnapshotState(const PlayerSnapshotState& state)
	{
		ObjectTransform.Position = state.Position;
		ObjectTransform.SetYaw(state.Yaw);
		// TODO: pitch

		m_AnimationController.PlayAnimation(state.AnimationIndex);
	}
	void PlayerObject::PreDraw()
	{
		// Draw Head
		Matrix4x4 headMatrix = ObjectTransform.GetMatrix();
		Matrix4x4 posMat(1.f);
		posMat = glm::translate(posMat, Vector3(0.0f, 2.7f, 0.0f));

		headMatrix *= posMat;

		m_HeadModel->Draw(Resources.GetShader("Model"), headMatrix);

		// Set Animation
		m_AnimationController.PlayAnimation(m_AnimationController.GetCurrentAnimationIndex());
	}
}