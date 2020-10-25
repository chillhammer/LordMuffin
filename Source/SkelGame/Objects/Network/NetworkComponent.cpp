#include "SkelPCH.h"
#include <Client/ClientManager.h>
#include <Server/ServerManager.h>
#include <GameObject/GameObjectManager.h>
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

	NetworkComponent::NetworkComponent() : m_LocalPlayer(nullptr)
	{
		for (int i = 0; i < Net::MAX_PLAYERS; ++i)
		{
			m_PlayerObjects[i] = nullptr;
		}
	}

	void Skel::NetworkComponent::OnSceneCreated()
	{
#ifndef SERVER
		// Create local player even if not connected to server
		GameObject* localPlayer = Objects.InstantiateObject(Resources.GetPrefab("Player"));
		SetLocalPlayerObject(localPlayer);
		Client.ClientSubject.AddObserver(this);
#else
		Server.GetClientHandler().ClientSubject.AddObserver(this);
#endif

	}

	void NetworkComponent::Update()
	{
		// Handle the appropriate packett. Their implementations are in another file
#ifdef SERVER
		Server_HandlePackets();
#else
		Client_HandlePackets();
#endif
	}
	GameObject* Skel::NetworkComponent::GetPlayerObject(uint16 clientID) const
	{
		return m_PlayerObjects[clientID];
	}
	GameObject* Skel::NetworkComponent::GetLocalPlayerObject() const
	{
		if (Client.Connected())
		{
			ASSERT(m_PlayerObjects[Client.GetClientID()] == m_LocalPlayer, "Must be the same object");
		}
		return m_LocalPlayer;
	}
	// Creates player object with certain clientID
	GameObject* Skel::NetworkComponent::CreatePlayerObject(uint16 clientID)
	{
		ASSERT(m_PlayerObjects[clientID] == nullptr, "Cannot overwrite player object");
		GameObject* playerObj = Objects.InstantiateObject(Resources.GetPrefab("Player"));
		SetPlayerObject(clientID, playerObj);
		return playerObj;
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

#ifndef SERVER
		if (Client.Connected() && clientID == Client.GetClientID())
		{
			m_LocalPlayer = obj;
		}
#endif
	}
	void Skel::NetworkComponent::SetLocalPlayerObject(GameObject* obj)
	{
		m_LocalPlayer = obj;
		if (Client.Connected())
		{
			m_PlayerObjects[Client.GetClientID()] = obj;
		}
	}

	// Events
	void Skel::NetworkComponent::OnEvent(const Subject* subject, Event& event)
	{
		Evnt::Dispatch<ClientConnectEvent>(event, EVENT_BIND_FN(NetworkComponent, OnClientConnect));
		Evnt::Dispatch<ClientDisconnectEvent>(event, EVENT_BIND_FN(NetworkComponent, OnClientDisconnect));
	}
	bool NetworkComponent::OnClientConnect(ClientConnectEvent e)
	{
		// Set from local
#ifndef SERVER
		if (e.ClientID == Client.GetClientID() && m_LocalPlayer)
		{
			SetPlayerObject(Client.GetClientID(), m_LocalPlayer);
			return false;
		}
#endif

		GameObject* playerObj = GetPlayerObject(e.ClientID);
		// Spawn player
		if (playerObj != nullptr)
		{
			playerObj->Destroy();
			SetPlayerObject(e.ClientID, nullptr);
		}
		CreatePlayerObject(e.ClientID);

		return false;
	}
	bool NetworkComponent::OnClientDisconnect(ClientDisconnectEvent e)
	{
		GameObject* clientObj = GetPlayerObject(e.ClientID);
		if (m_LocalPlayer == clientObj)
		{
			// Clear all players
			for (int i = 0; i < Net::MAX_PLAYERS; i++)
			{
				GameObject* obj = m_PlayerObjects[i];
				if (obj && obj != m_LocalPlayer)
				{
					obj->Destroy();
				}
				m_PlayerObjects[i] = nullptr;
			}
		}
		else
		{
			// Clear specific player
			if (clientObj)
			{
				clientObj->Destroy();
				m_PlayerObjects[e.ClientID] = nullptr;
			}
		}
		return false;
	}
}
