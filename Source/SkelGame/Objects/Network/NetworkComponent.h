#pragma once
#include <Components/GameObjectComponent.h>
#include <Net/Net.h>

namespace Skel
{
	class NetworkComponent : public GameObjectComponent
	{
		NetworkComponent();

		void Update();

		RTTR_ENABLE(GameObjectComponent)
		RTTR_REGISTRATION_FRIEND
	private:
		void HandleClientPackets();
		void HandleServerPackets();

		GameObject* m_PlayerObjects[MAX_PLAYERS];
	};
}