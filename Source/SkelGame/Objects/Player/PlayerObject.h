#pragma once
#include "GameObject/GameObject.h"
#include <Graphics/Model/ModelAnimationController.h>
#include "PlayerInputState.h"

namespace Skel
{
	// Each value represents the animation index
	enum PlayerAnimation {
		TPose = 0, Stand = 1
	};

	// GameObject representing player. Handles movement/animation
	class PlayerObject : public GameObject
	{
	public:
		PlayerObject();

		void ProcessInput(const PlayerInputState& input, float dt);
		void ProcessAnimation(const PlayerInputState& input);
		void ApplySnapshotState(const class PlayerSnapshotState& state);
		uint8 GetAnimation() const { return m_AnimationController.GetCurrentAnimationIndex(); }
		virtual void PreDraw() override;
	private:
		ModelAnimationController m_AnimationController;

		ModelPtr m_HeadModel;
	};
}