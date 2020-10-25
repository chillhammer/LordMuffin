#pragma once
#include <Net/Net.h>
#include <Net/Address.h>
#include <EventSystem/Subject.h>

namespace Skel::Net
{
	struct ClientSlot
	{
		uint16 ID;
		Address ClientAddress;
		ClientSlot(uint16 id, Address address) : ID(id), ClientAddress(address) {}
	};

	// Handles the player management from the server perspective
	// Note an optimization can be made as to making the active PlayerObjects always adjacent in memory
	class ClientHandler
	{
	public:
		ClientHandler();

		uint16 ActivePlayers() const { return m_ActivePlayers; }
		uint16 RemainingSlots() const { return MAX_PLAYERS - m_ActivePlayers; }
		uint16 AddPlayer(Address address);
		uint16 GetClientIndex(const Address& address) const;
		bool ClientExists(const Address& address) const;
		uint64 GetClientTick(uint16 clientIndex) const;
		uint64 GetClientLastReceivedTick(uint16 clientIndex) const;
		void   UpdateClientTick(uint16 clientIndex, uint64 tick);
		bool   TryInputAck(uint16 clientIndex, uint64 tick);
		void RemovePlayer(uint16 clientIndex);
		bool IsActive(uint16 clientID) const;
		const class GameObject* GetPlayerObject(uint16 clientID) const;
		const std::vector<ClientSlot>& GetClientSlots() const;

		Subject ClientSubject;

	private:
		// Helper Functions
		uint16 FindAvailableSlotIndex() const;

	private:
		bool m_SlotAvailability[MAX_PLAYERS];
		uint64 m_LatestTick[MAX_PLAYERS];
		uint64 m_LastReceivedOnServerTick[MAX_PLAYERS];
		uint64 m_InputAcks[MAX_PLAYERS];


		std::vector<ClientSlot> m_ClientSlots; // List of active player addresses
		uint16 m_ActivePlayers = 0;
	};
}