#include "SkelPCH.h"
#include "PlayerObject.h"

namespace Skel {
	void PlayerObject::ProcessInput(const PlayerInputState& input, float dt)
	{
		float forward = (input.Forward ? 1 : 0) - (input.Back ? 1 : 0);
		ObjectTransform.Position += ObjectTransform.GetHeading() * forward * dt;
	}
}