#include "SkelPCH.h"
#include "PlayerObject.h"
#include "PlayerSnapshotState.h"

namespace Skel {
	void PlayerObject::ProcessInput(const PlayerInputState& input, float dt)
	{
		float forward = (input.Forward ? 1 : 0) - (input.Back ? 1 : 0);
		ObjectTransform.Position += ObjectTransform.GetHeading() * forward * dt;
	}
	void PlayerObject::ApplySnapshotState(const PlayerSnapshotState& state)
	{
		ObjectTransform.Position = state.Position;
		// TODO: pitch and yaw
	}
}