#pragma once
#include <Components/GameObjectComponent.h>
#include <Objects/Player/PlayerComponent.h>
#include <EventSystem/Events/NetEvent.h>
#include <Net/Net.h>

namespace Skel
{
	class NetworkComponent : public GameObjectComponent, IObserver
	{
	public:
		NetworkComponent();

		virtual void OnSceneCreated() override;
		virtual void Update() override;

		GameObject* GetPlayerObject(uint16 clientID) const;
		GameObject* GetLocalPlayerObject() const;
		GameObject* CreatePlayerObject(uint16 clientID);
		PlayerComponent* GetPlayerComponent(uint16 clientID) const;
		uint16 GetPlayerID(GameObject*) const;
		void SetPlayerObject(uint16 clientID, GameObject* obj);
		void SetLocalPlayerObject(GameObject* obj);

		virtual void OnEvent(const Subject* subject, class Event& event);


		RTTR_ENABLE(GameObjectComponent)
		RTTR_REGISTRATION_FRIEND
	private:
		void Client_HandlePackets();
		void Server_HandlePackets();

		bool OnClientConnect(ClientConnectEvent e);
		bool OnClientDisconnect(ClientDisconnectEvent e);

		GameObject* m_PlayerObjects[Net::MAX_PLAYERS];
		

		// Server
		double m_TimeSinceSnapshotSent = 0;

		// Client
		GameObject* m_LocalPlayer;
	};
}