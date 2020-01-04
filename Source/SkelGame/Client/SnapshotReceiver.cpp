#include "SkelPCH.h"
#include "SnapshotReceiver.h"
#include <Client/ClientManager.h>
#include <Game/GameManager.h>
namespace Skel::Net {

	void SnapshotReceiver::ReceiveSnapshotPacket(const PlayerSnapshotPacket& packet)
	{
		uint64 tick = packet.TickNumber;
		double snapshotTime = packet.Timestamp - Client.GetSynchronizer().Offset() + Client.GetSynchronizer().Latency() + 0.1;

		if (tick > m_LastReceivedTick) {
			m_LastReceivedTick = tick;

			//LOG("SnapshotTime: {0}\tTime: {1}", snapshotTime, Game.RunningTime());

			m_ActiveClients.clear();

			const auto& entries = packet.GetSnapshotEntries();

			m_ReceivedStates.emplace(entries, snapshotTime);

			for (const SnapshotEntry& entry : entries) {
				PlayerObject& playerObj = m_PlayerObjectArray[entry.ClientID];

				//ApplySnapshotState(entry.State, playerObj);

				m_ActiveClients.push_back(entry.ClientID);
			}
		}
	}

	// Updates world state by using snapshot data
	void SnapshotReceiver::Update()
	{
		// TODO: Snapshot Receiver Update
	}

	// Applies snapshot to object directly. No interpolation
	void SnapshotReceiver::ApplySnapshotState(const PlayerSnapshotState& state, PlayerObject& player)
	{
		player.ObjectTransform.Position = state.Position;
		// TODO: pitch and yaw
	}

}