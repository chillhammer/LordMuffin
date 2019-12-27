#pragma once
#include <Objects/Player/PlayerSnapshotState.h>
#include <Packets/SnapshotPacket.h>

namespace Skel::Net
{
	// Receives Snapshot from packet and handles effect on clients
	class SnapshotReceiver
	{
	public:
		void ReceiveSnapshotPacket(const PlayerSnapshotPacket& packet);
		void ApplySnapshotState(const PlayerSnapshotState& state, PlayerObject& player);
		void SetPlayerObjectArray(class PlayerObject* arr) { m_PlayerObjectArray = arr; }
		const std::vector<uint16>& GetActiveClients() const { return m_ActiveClients; }
	private:
		PlayerSnapshotState m_LastReceivedState;
		uint64 m_LastReceivedTick = 0;
		class PlayerObject* m_PlayerObjectArray; // Active/Non-active player objects
		std::vector<uint16> m_ActiveClients;
	};
}