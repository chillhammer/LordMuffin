#include "SkelPCH.h"
#include <Objects/Player/PlayerObject.h>
#include "ClientHandler.h"

namespace Skel::Net {
	ClientHandler::ClientHandler()
	{
		for (int i = 0; i < MAX_PLAYERS; ++i) {
			m_SlotAvailability[i] = true;

			m_ClientSlots[i].ID = i;
		}
	}
	uint16 ClientHandler::AddPlayer()
	{
		ASSERT(RemainingSlots() > 0, "No remaining slots when trying to add player");

		uint16 playerIndex = FindAvailableSlotIndex();
		ASSERT(m_SlotAvailability[playerIndex] == true, "Cannot add to filled up slot");
		m_SlotAvailability[playerIndex] = false;

		m_ActivePlayers++;

		return playerIndex;
	}
	void ClientHandler::RemovePlayer(uint16 clientIndex)
	{
		ASSERT(!m_SlotAvailability[clientIndex], "Cannot remove empty slot");

		m_SlotAvailability[clientIndex] = true;

		m_ActivePlayers--;
	}
	uint16 ClientHandler::FindAvailableSlotIndex() const
	{
		for (int i = 0; i < MAX_PLAYERS; ++i) {
			if (m_SlotAvailability[i]) {
				return i;
			}
		}
		return -1;
	}
}