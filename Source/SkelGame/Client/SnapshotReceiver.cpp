#include "SkelPCH.h"
#include "SnapshotReceiver.h"
#include <Client/ClientManager.h>
#include <Game/GameManager.h>
#include <Objects/Player/PlayerPredictionStateHistory.h>
namespace Skel::Net {

	void SnapshotReceiver::ReceiveSnapshotPacket(const PlayerSnapshotPacket& packet)
	{
		uint64 tick = packet.TickNumber;
		double snapshotTime = packet.Timestamp - Client.GetSynchronizer().Offset() + std::max(0.0, /*- Client.GetSynchronizer().Latency()*/ + SNAPSHOT_INTER_BUFFER);

		if (tick > m_LastReceivedServerTick && Client.GetSynchronizer().IsSynchronized()) {
			m_LastReceivedServerTick = tick;
			m_LastReceivedClientTick = packet.ClientTickNumber;

			m_ActiveClients.clear();

			const auto& entries = packet.GetSnapshotEntries();

			// Validate Client-Side Prediction
			auto& playerEntry = std::find_if(entries.begin(), entries.end(), [](const SnapshotEntry& s) { return s.ClientID == Client.GetClientID(); });

			// Check against predicted output and do corrections. Updates player object state if needed
			double calculatedClientTime = Game.RunningTime() - (Client.GetSynchronizer().Latency() * 2 + Net::SNAPSHOT_INTER_BUFFER);
			// Calculated time is off... I think i should send prediction ID instead
			Client.GetPredictionHistory().CorrectState(playerEntry->State, m_PlayerObjectArray[playerEntry->ClientID], calculatedClientTime, m_LastReceivedClientTick);

			// Add to queue to update the world state in Update()
			m_ReceivedStates.emplace(entries, snapshotTime);
		}
	}

	// Updates world state by using snapshot data
	// Note an optimization would be to not have SnapshotRecords be copy initialized
	void SnapshotReceiver::Update()
	{

		// First Snapshot, No Interpolation
		if (!m_ReceivedFirst && !m_ReceivedStates.empty()) {
			m_ReceivedFirst = true;
			m_BaseSnapshot = m_ReceivedStates.front();
			m_ReceivedStates.pop();
		}

		double baseTime = m_BaseSnapshot.Time; // Time of snapshot you are interpolating from
		double gameTime = Game.RunningTime();

		if (baseTime < gameTime && !m_ReceivedStates.empty()) {
			SnapshotRecord next = m_ReceivedStates.front();
			// Find valid next & update base snapshot
			while (gameTime > next.Time)
			{
				ASSERT(next.Time > baseTime, "Snapshot times must be in order");
				m_BaseSnapshot = next;
				m_ReceivedStates.pop();
				if (m_ReceivedStates.empty())
					break;
				next = m_ReceivedStates.front();
			}

			// Cannot find valid next or trying to catch up to new base snapshot
			if (m_ReceivedStates.empty() || m_BaseSnapshot.Time > gameTime) {
				if (m_BaseSnapshot.Time > gameTime)
					LOG_WARN("Trying to catch up to base snapshot time ({0}), current time: {1}", m_BaseSnapshot.Time, gameTime);
				m_Interpolating = false;
			}
			// Set Target Snapshot
			else {
				m_TargetSnapshot = m_ReceivedStates.front();
				m_Interpolating = true;
			}
		}
		// Catch up to base snapshot
		else if (baseTime > gameTime) {
			m_Interpolating = false;
		}

		// Update world state
		if (m_ReceivedFirst)
		{
			if (m_Interpolating) {
				// Apply interpolated snapshot
				auto& entries = InterpolateRecords(m_BaseSnapshot, m_TargetSnapshot);
				ApplySnapshotEntries(entries);
			}
			else {
				// Apply base snapshot
				const auto& entries = m_BaseSnapshot.Entries;
				ApplySnapshotEntries(entries);
				//LOG("Stuck at base snapshot: {0}", m_BaseSnapshot.Time);
			}
		}
	}

	// Applies snapshot to object directly. No interpolation
	void SnapshotReceiver::ApplySnapshotState(const PlayerSnapshotState& state, PlayerObject& player)
	{
		player.ApplySnapshotState(state);
	}

	// Apply list of snapshot entries. Runs once a tick
	// Will handle Player edge-case for client-side prediction validation
	void SnapshotReceiver::ApplySnapshotEntries(const std::vector<SnapshotEntry>& entries)
	{
		m_ActiveClients.clear();

		for (const SnapshotEntry& entry : entries) {
			m_ActiveClients.push_back(entry.ClientID);
			PlayerObject& playerObj = m_PlayerObjectArray[entry.ClientID];

			if (entry.ClientID == Client.GetClientID()) {
				

				continue;
			}
			ApplySnapshotState(entry.State, playerObj);
		}
	}

	std::vector<SnapshotEntry> SnapshotReceiver::InterpolateRecords(const SnapshotRecord& a , const SnapshotRecord& b)
	{
		std::vector<SnapshotEntry> result;
		result.reserve(a.Entries.size());

		uint16 aIndex = 0;
		uint16 bIndex = 0;
		double gameTime = Game.RunningTime();
		float t = (gameTime - a.Time) / (b.Time - a.Time);
		//ASSERT(t <= 1, "t must be below 1");
		//ASSERT(t >= 0, "t must be above 0");
		uint16 aEntities = a.Entries.size();
		uint16 bEntities = b.Entries.size();
		while (aIndex < aEntities && bIndex < bEntities)
		{
			uint16 aID = a.Entries[aIndex].ClientID;
			uint16 bID = b.Entries[bIndex].ClientID;
			if (aID > bID) {
				++bIndex;
			}
			else if (aID < bID) {
				++aIndex;
			}
			else {
				// Interpolate
				PlayerSnapshotState interpolatedState;
				interpolatedState.Position = (b.Entries[bIndex].State.Position - a.Entries[aIndex].State.Position) * t
					+ a.Entries[aIndex].State.Position;

				interpolatedState.Yaw = LerpAngle(a.Entries[aIndex].State.Yaw, b.Entries[bIndex].State.Yaw, t);
				interpolatedState.Pitch = LerpAngle(a.Entries[aIndex].State.Pitch, b.Entries[bIndex].State.Pitch, t);

				result.emplace_back(aID, interpolatedState);
				++aIndex; ++bIndex;
			}
		}

		return result;
	}

}