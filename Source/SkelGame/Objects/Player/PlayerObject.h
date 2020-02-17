#pragma once
#include "GameObject/GameObject.h"
#include "PlayerInputState.h"

namespace Skel
{
	class PlayerObject : public GameObject
	{
	public:
		PlayerObject();

		void ProcessInput(const PlayerInputState& input, float dt);
		void ApplySnapshotState(const class PlayerSnapshotState& state);
		virtual void PreDraw() override;
	private:
		ModelPtr m_HeadModel;
	};
}