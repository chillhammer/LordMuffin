#include "SkelPCH.h"
#include <Client/ClientManager.h>
#include <Resources/ResourceManager.h>
#include "NetworkComponent.h"

namespace Skel
{
	RTTR_REGISTRATION
	{
		using namespace rttr;
		registration::class_<NetworkComponent>("Network")
			.constructor<>(registration::public_access);
	}

	NetworkComponent::NetworkComponent()
	{
		for (int i = 0; i < Net::MAX_PLAYERS; ++i)
		{
			m_PlayerObjects[i] = nullptr;
		}
	}

	void NetworkComponent::Update()
	{
		// Handle the appropriate packett. Their implementations are in another file
#ifdef SERVER
		HandleServerPackets();
#else
		HandleClientPackets();
#endif
	}
	GameObject* Skel::NetworkComponent::GetPlayerObject(uint16 clientID) const
	{
		return m_PlayerObjects[clientID];
	}
	// Creates player object with certain clientID
	GameObject* Skel::NetworkComponent::CreatePlayerObject(uint16 clientID)
	{
		ASSERT(m_PlayerObjects[clientID] == nullptr, "Cannot overwrite player object");
		GameObject* playerObj = Game.InstantiateObject(Resources.GetPrefab("Player"));
		SetPlayerObject(clientID, playerObj);
		m_PlayerObjectArray[entry.ClientID] = playerObj;											// TODO: put m_PlayerObjectArray only in Network component. Spawn from there
		Objects::FindFirstComponent<NetworkComponent>().SetPlayerObject(entry.ClientID, playerObj); // TODO: replace with spawn event

		return nullptr;
	}
	PlayerComponent* Skel::NetworkComponent::GetPlayerComponent(uint16 clientID) const
	{
		auto obj = GetPlayerObject(clientID);
		if (obj && obj->HasComponent<PlayerComponent>())
		{
			return &obj->GetComponent<PlayerComponent>();
		}
		return nullptr;
	}
	void Skel::NetworkComponent::SetPlayerObject(uint16 clientID, GameObject* obj)
	{
		ASSERT((obj && m_PlayerObjects[clientID] == nullptr) || ((obj == nullptr && m_PlayerObjects[clientID])), "Cannot overwrite player");
		m_PlayerObjects[clientID] = obj;
		if (Client.Connected() && clientID == Client.GetClientID())
		{
			ASSERT(m_LocalPlayer == nullptr || obj == nullptr, "Cannot overwrite local player");
			m_LocalPlayer = obj;
		}
	}
	void Skel::NetworkComponent::SetLocalPlayerObject(GameObject* obj)
	{
		ASSERT(m_LocalPlayer == nullptr || obj == nullptr, "Cannot overwrite local player");
		m_LocalPlayer = obj;
		if (Client.Connected())
		{
			m_PlayerObjects[Client.GetClientID()] = obj;
		}
	}
}
