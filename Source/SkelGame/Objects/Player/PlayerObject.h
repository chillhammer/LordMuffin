#pragma once
#include "GameObject/GameObject.h"
#include "PlayerInputState.h"

namespace Skel
{
	class PlayerObject : public GameObject
	{
	public:
		PlayerObject() : GameObject("WoodenBox") {}

		void ProcessInput(const PlayerInputState& input, float dt);
		void ApplySnapshotState(const class PlayerSnapshotState& state);
	};
}