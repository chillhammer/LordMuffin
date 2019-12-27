#include "SkelPCH.h"
#include <Objects/Player/PlayerObject.h>
#include "ClientHandler.h"

namespace Skel::Net {
	ClientHandler::ClientHandler()
	{
		m_ClientSlots.reserve(MAX_PLAYERS);
		for (int i = 0; i < MAX_PLAYERS; ++i) {
			m_SlotAvailability[i] = true;
		}
	}
	uint16 ClientHandler::AddPlayer(Address playerAddress)
	{
		ASSERT(RemainingSlots() > 0, "No remaining slots when trying to add player");

		uint16 playerIndex = FindAvailableSlotIndex();
		ASSERT(m_SlotAvailability[playerIndex] == true, "Cannot add to filled up slot");
		m_SlotAvailability[playerIndex] = false;

		m_ClientSlots.emplace_back(playerIndex, playerAddress);

		m_ActivePlayers++;

		return playerIndex;
	}
	void ClientHandler::RemovePlayer(uint16 clientIndex)
	{
		ASSERT(!m_SlotAvailability[clientIndex], "Cannot remove empty slot");

		m_SlotAvailability[clientIndex] = true;

		auto toBeRemoved = std::find_if(m_ClientSlots.begin(), m_ClientSlots.end(),
			[&index = clientIndex] (const ClientSlot& s) -> bool { return index == s.ID; });
		
		m_ClientSlots.erase(toBeRemoved);

		m_ActivePlayers--;
	}
	bool ClientHandler::IsActive(uint16 clientID) const
	{
		return !m_SlotAvailability[clientID];
	}
	const PlayerObject* ClientHandler::GetPlayerObject(uint16 clientID) const
	{
		if (!IsActive(clientID))
			return nullptr;
		return &m_PlayerObjectArray[clientID];
	}
	// Used to get addresses of players
	const std::vector<ClientSlot>& ClientHandler::GetClientSlots() const
	{
		return m_ClientSlots;
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