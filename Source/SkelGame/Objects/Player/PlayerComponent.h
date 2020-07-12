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
		bool IsLocalClient() const;

		virtual void OnCreated() override;
		//virtual void Update() override;
		virtual void Draw() override;

		RTTR_ENABLE(GameObjectComponent)
		RTTR_REGISTRATION_FRIEND
	private:
		class NetworkComponent* m_Network;
	};
}
