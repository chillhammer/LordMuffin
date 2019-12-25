#pragma once
#include <Objects/Player/PlayerObject.h>
#include <Net/Net.h>

namespace Skel::Net
{
	struct ClientSlot
	{
		uint16 ID;

	};

	// Handles the player management from the server perspective
	// Note an optimization can be made as to making the active PlayerObjects always adjacent in memory
	class ClientHandler
	{
	public:
		ClientHandler();

		uint16 ActivePlayers() const { return m_ActivePlayers; }
		uint16 RemainingSlots() const { return MAX_PLAYERS - m_ActivePlayers; }
		uint16 AddPlayer();
		void RemovePlayer(uint16 clientIndex);
		void SetPlayerObjectArray(class PlayerObject* arr) { m_PlayerObjectArray = arr; }

	private:
		// Helper Functions
		uint16 FindAvailableSlotIndex() const;

	private:
		bool m_SlotAvailability[MAX_PLAYERS];
		ClientSlot m_ClientSlots[MAX_PLAYERS];
		class PlayerObject* m_PlayerObjectArray;
		uint16 m_ActivePlayers = 0;
	};
}