#pragma once
#include "Components/GameObjectComponent.h"
#include "PlayerInputState.h"


namespace Skel
{
	class PlayerComponent : public GameObjectComponent
	{
	public:
		PlayerComponent();
		void ProcessInput(const PlayerInputState& input, float dt);
		void ApplySnapshotState(const class PlayerSnapshotState& state);

		virtual void Update() override;

		RTTR_ENABLE(GameObjectComponent)
		RTTR_REGISTRATION_FRIEND
	};
}
