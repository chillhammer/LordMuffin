#include "SkelPCH.h"
#include "PlayerObject.h"
#include "PlayerSnapshotState.h"

namespace Skel {
	void PlayerObject::ProcessInput(const PlayerInputState& input, float dt)
	{
		float forward = (input.Forward ? 1 : 0) - (input.Back ? 1 : 0);
		ObjectTransform.Position += ObjectTransform.GetHeading() * forward * dt;
		
		ObjectTransform.Position += ObjectTransform.GetSide() * (input.Jump ? 1.f : 0.f);
	}
	void PlayerObject::ApplySnapshotState(const PlayerSnapshotState& state)
	{
		ObjectTransform.Position = state.Position;
		// TODO: pitch and yaw
	}
}