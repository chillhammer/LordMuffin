#include "SkelPCH.h"
#include "SnapshotReceiver.h"
namespace Skel::Net {

	void SnapshotReceiver::ReceiveSnapshotPacket(const PlayerSnapshotPacket& packet)
	{
		uint64 tick = packet.TickNumber;

		if (tick > m_LastReceivedTick) {
			m_LastReceivedTick = tick;
			m_ActiveClients.clear();

			auto entries = packet.GetSnapshotEntries();
			for (const SnapshotEntry& entry : entries) {
				PlayerObject& playerObj = m_PlayerObjectArray[entry.ClientID];

				ApplySnapshotState(entry.State, playerObj);

				m_ActiveClients.push_back(entry.ClientID);
			}
		}
	}

	// Applies snapshot to object directly. No interpolation
	void SnapshotReceiver::ApplySnapshotState(const PlayerSnapshotState& state, PlayerObject& player)
	{
		player.ObjectTransform.Position = state.Position;
		// TODO: pitch and yaw
	}

}