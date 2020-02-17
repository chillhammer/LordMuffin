#include "SkelPCH.h"
#include "PlayerObject.h"
#include <Resources/ResourceManager.h>
#include "PlayerSnapshotState.h"

namespace Skel {
	PlayerObject::PlayerObject() : GameObject("Soldier"), m_HeadModel(Resources.GetModel("SoldierHead")) {}

	void PlayerObject::ProcessInput(const PlayerInputState& input, float dt)
	{
		float forward = (input.Forward ? 1 : 0) - (input.Back ? 1 : 0);

		ObjectTransform.SetYaw(input.Yaw);
		ObjectTransform.Position += ObjectTransform.GetHeading() * forward * dt;
		
		ObjectTransform.Position += ObjectTransform.GetSide() * (input.Jump ? 1.f : 0.f);
	}
	void PlayerObject::ApplySnapshotState(const PlayerSnapshotState& state)
	{
		ObjectTransform.Position = state.Position;
		ObjectTransform.SetYaw(state.Yaw);
		// TODO: pitch
	}
	void PlayerObject::PreDraw()
	{
		Matrix4x4 headMatrix = ObjectTransform.GetMatrix();
		Matrix4x4 posMat(1.f);
		posMat = glm::translate(posMat, Vector3(0.0f, 2.7f, 0.0f));

		headMatrix *= posMat;

		m_HeadModel->Draw(Resources.GetShader("Model"), headMatrix);
	}
}